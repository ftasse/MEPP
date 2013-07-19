#ifndef MESHSIMPLIFICATIONHXX
#define MESHSIMPLIFICATIONHXX

//#include <QInputDialog>
#include <Eigen/Core>
#include <iostream>
#include <queue>
#include <float.h>

#include "mesh2skeleton.h"

typedef MeshToSkeleton::VertexId VertexId;
typedef MeshToSkeleton::HEdgeId HEdgeId;
typedef MeshToSkeleton::FaceId FaceId;
typedef MeshToSkeleton Skel;

class Simplification
{
public:
    MeshToSkeleton& skel;

    Simplification(MeshToSkeleton& skel_);
    void run(double wa = 1.0, double wb = 0.1, bool use_plane_quadrics = false, bool interactive = false);
};

template <class M>
MeshToSkeleton simplify(M &mesh, double wa, double wb, bool use_plane_quadrics)
{
	MeshToSkeleton mesh2skeleton(mesh);
	Simplification simplification(mesh2skeleton);
	simplification.run(wa, wb, use_plane_quadrics);
	return mesh2skeleton;
}


class CustomDecimation
{
    typedef struct CollapseInfo
    {
        HEdgeId heid;
        int updateLevel;
        double shapeCost, samplingCost;

        CollapseInfo() {
            updateLevel = 0;
            shapeCost = samplingCost = DBL_MAX;
        }

        double error() const
        {
            return shapeCost+samplingCost;
        }

    } CollapseInfo;

    class collapseinfo_cmp
    {
        bool reverse;
    public:
        collapseinfo_cmp(const bool& revparam=false)
        {reverse=revparam;}
        bool operator() (const CollapseInfo& lhs, const CollapseInfo&rhs) const
        {
            if (reverse) return (lhs.error() < rhs.error());
            else return (lhs.error() > rhs.error());
        }
    };

public:
    CustomDecimation(MeshToSkeleton &skel_, double wa_ = 1.0, double wb_ = 0.1, bool use_plane_quadrics_ = false):skel(skel_), wa(wa_), wb(wb_), use_plane_quadrics(use_plane_quadrics_) {}

    void computeQErrorMatrices(MeshToSkeleton &skel_)
    {
        for (Skel::VertexIter v_it=skel_.vertices_begin(); v_it!=skel_.vertices_end(); ++v_it)
        {
            (*v_it).Q = Eigen::MatrixXd::Zero(4, 4);
        }

        if (!use_plane_quadrics )
            for (Skel::HEdgeConstIter h_it=skel_.hedges_begin(); h_it!=skel_.hedges_end(); ++h_it)
            {
                Eigen::MatrixXd K, Kp;
                OpenMesh::Vec3d a, b;

                K = Eigen::MatrixXd::Zero(3, 4);
                a = skel_.vertex(h_it->verts[1])->point - skel_.vertex(h_it->verts[0])->point;
                a.normalize_cond();
                b = OpenMesh::cross(a, skel_.vertex(h_it->verts[0])->point);
                K <<    0,  -a[2],  a[1],   -b[0],
                        a[2],      0, -a[0],   -b[1],
                        -a[1],   a[0],    0,    -b[2];
                Kp = K.transpose()*K;
                skel_.vertex(h_it->verts[0])->Q += Kp;
                skel_.vertex(h_it->verts[1])->Q += Kp;
            }
        else
            for (Skel::FaceConstIter f_it = skel_.faces_begin(); f_it!=skel_.faces_end(); ++f_it)
            {
                HEdgeId hedges[3] = {f_it->hedges[0], f_it->hedges[1], f_it->hedges[2]};
                VertexId verts[3];
                OpenMesh::Vec3d v[3];
                for (int k=0; k<3; ++k)
                {
                    verts[k] = skel_.hedge(hedges[k])->verts[0];
                    v[k] = skel_.vertex(verts[k])->point;
                }

                OpenMesh::Vec3d n = (v[1]-v[0]) % (v[2]-v[0]);
                double area = n.norm();
                if (area > FLT_MIN)
                {
                    n /= area;
                    area *= 0.5;
                }

                Eigen::Matrix4d K;
                double tmp = -(v[0] | n);
                K << n[0]*n[0] , n[0]*n[1], n[0]*n[2], n[0]*tmp,
                        0      , n[1]*n[1], n[1]*n[2], n[1]*tmp,
                        0      , 0        , n[2]*n[2], n[2]*tmp,
                        0      , 0        , 0        , tmp*tmp;
                K = K*area;
                skel_.vertex(verts[0])->Q += K;
                skel_.vertex(verts[1])->Q += K;
                skel_.vertex(verts[2])->Q += K;
            }
    }

    double getShapeEdgeCost(CollapseInfo ci)
    {
        VertexId i = skel.hedge(ci.heid)->verts[0];
        VertexId j = skel.hedge(ci.heid)->verts[1];
        OpenMesh::Vec3d v = skel.vertex(j)->point;
        Eigen::Vector4d p(v[0], v[1], v[2], 1);
        double error = 0.0;

        if (!use_plane_quadrics )
        {
            error = p.transpose() * skel.vertex(i)->Q * p;
            error += p.transpose() * skel.vertex(j)->Q * p ;
        }
        else
        {
            Eigen::MatrixXd Q = skel.vertex(i)->Q + skel.vertex(j)->Q;
            error =  Q(0,0)*p[0]*p[0] + 2.0*Q(0,1)*p[0]*p[1] + 2.0*Q(0,2)*p[0]*p[2] + 2.0*Q(0,3)*p[0]*p[3]
                            + Q(1,1)*p[1]*p[1] + 2.0*Q(1,2)*p[1]*p[2] + 2.0*Q(1,3)*p[1]*p[3]
                            +     Q(2,2)*p[2]*p[2] + 2.0*Q(2,3)*p[2]*p[3]
                            +     Q(3,3)*p[3]*p[3];
        }
        return wa*error;
    }

    double getSamplingEdgeCost(CollapseInfo ci)
    {
        VertexId i = skel.hedge(ci.heid)->verts[0];
        VertexId j = skel.hedge(ci.heid)->verts[1];

        OpenMesh::Vec3d vj = skel.vertex(j)->point, vi = skel.vertex(i)->point;
        double cost = 0.0f;

        for (std::set<HEdgeId>::iterator oh_it = skel.vertex(i)->out_hedges.begin(); oh_it!=skel.vertex(i)->out_hedges.end(); ++oh_it)
        {
            if (skel.hedge(*oh_it)->verts[1] != j)
            {
                OpenMesh::Vec3d vk = skel.vertex(skel.hedge(*oh_it)->verts[1])->point;
                cost += (vi-vk).norm();
            }
        }
        cost *= (vi-vj).norm();
        return wb*cost;
    }

    CollapseInfo create_collapseinfo(HEdgeId heid, bool update = false)
    {
        if (update)
            skel.hedge(heid)->updateLevel++;
        else
            skel.hedge(heid)->updateLevel = 0;

        CollapseInfo ci;
        ci.heid = heid;
        ci.updateLevel = skel.hedge(heid)->updateLevel;
        ci.samplingCost = getSamplingEdgeCost(ci);
        ci.shapeCost = getShapeEdgeCost(ci);
        return ci;
    }

    void collapse(CollapseInfo ci)
    {
        VertexId i = skel.hedge(ci.heid)->verts[0];
        VertexId j = skel.hedge(ci.heid)->verts[1];

        skel.vertex(j)->Q = skel.vertex(j)->Q + skel.vertex(i)->Q;
        skel.vertex(j)->record.insert(skel.vertex(j)->record.end(), skel.vertex(i)->record.begin(), skel.vertex(i)->record.end());
        skel.vertex(i)->record.clear();
        skel.collapse_edge(ci.heid);
        //std::cout << "Collapsed edge " << ci.heid << " " << i << " " << ci.error() << "\n" << std::flush;

        for (std::set<HEdgeId>::iterator oh_it = skel.vertex(j)->out_hedges.begin(); oh_it!=skel.vertex(j)->out_hedges.end(); ++oh_it)
        {
            HEdgeId hid = *oh_it;
            if (!skel.hedge(hid)->is_deleted && skel.hedge(hid)->face>=0 && !skel.face(skel.hedge(hid)->face)->is_deleted)
                minHeap.push(create_collapseinfo(hid, true));
        }
        for (std::set<HEdgeId>::iterator ih_it = skel.vertex(j)->in_hedges.begin(); ih_it!=skel.vertex(j)->in_hedges.end(); ++ih_it)
        {
            HEdgeId hid = *ih_it;
            if (!skel.hedge(hid)->is_deleted && skel.hedge(hid)->face>=0 && !skel.face(skel.hedge(hid)->face)->is_deleted)
                minHeap.push(create_collapseinfo(hid, true));
        }

    }

    void collapse_loop(int max_collapses = 0, bool checkTunnels = false)
    {
        //CurveSkeleton original = mesh;
        computeQErrorMatrices(skel);

        for (Skel::VertexIter v_it=skel.vertices_begin(); v_it!=skel.vertices_end(); ++v_it)
        {
            (*v_it).record.push_back(v_it->idx);
        }

        for (Skel::HEdgeConstIter h_it=skel.hedges_begin(); h_it!=skel.hedges_end(); ++h_it)
        {
            if (!h_it->is_deleted && h_it->face>=0 && !skel.face(h_it->face)->is_deleted)
                minHeap.push(create_collapseinfo(h_it->idx));
        }

        int ncollapsed=0, nmissed=0, ntunnels = 0;
        std::cout << "Done Computing edge costs\n" << std::flush;

        while (!minHeap.empty())
        {
            CollapseInfo top = minHeap.top();
            minHeap.pop();

            if(skel.hedge(top.heid)->is_deleted || skel.hedge(top.heid)->face < 0 || skel.face(skel.hedge(top.heid)->face)->is_deleted)
                continue;
            else if (top.updateLevel != skel.hedge(top.heid)->updateLevel)
                continue;

            if ((ncollapsed >= max_collapses))
                break;

            ++ncollapsed;
            collapse(top);
        }

        std::cout << "Collapses: " << ncollapsed << " / " << skel.num_vertices() <<  "\n" << std::flush;
        std::cout << "Tunnels: " << ntunnels << "\n" << std::flush;
        std::cout << "Illegal collapses: " << nmissed << "\n" << std::flush;
    }

private:
    MeshToSkeleton& skel;
    std::priority_queue<CollapseInfo, std::vector<CollapseInfo>, collapseinfo_cmp> minHeap;
    double wa, wb;
    bool use_plane_quadrics;
};


Simplification::Simplification(MeshToSkeleton &skel_):skel(skel_) {}

void Simplification::run(double wa, double wb, bool use_plane_quadrics, bool interactive)
{
    int nbef = skel.num_valid_hedges();
    int target_collapses = 0;

    /*if (interactive)
    {
        bool ok;
        int input = QInputDialog::getInt(NULL, QString("QInputDialog::getInteger()"),
                                         QString("Target vertex count:"), 100, 0, 10000, 1, &ok);
        if (ok) target_collapses = input;
    }*/

    int g = 1 - ((skel.num_vertices() - skel.num_hedges()/2 + skel.num_faces())/2);
    std::cout << "\nStart connected Surgery [g = " << g <<"] \n" << std::flush;

    CustomDecimation tmp(skel, wa, wb, use_plane_quadrics);
    tmp.collapse_loop(skel.num_vertices() -target_collapses, false);

    for (Skel::VertexIter v_it=skel.vertices_begin(); v_it!=skel.vertices_end(); ++v_it)
    {
        if (!(*v_it).is_deleted)
            skel.remove_duplicate_edges(v_it->idx);
    }
    int naft = skel.num_valid_hedges();

    std::cout << "Before half edges: " << nbef << "\n" << std::flush;
    std::cout << "After half edges: " << naft << "\n" << std::flush;

    g = 1 - ((skel.num_valid_vertices()- naft/2 + skel.num_valid_faces())/2);
    std::cout << "Stop connected Surgery [g = " << g <<"] \n" << std::flush;
}

#endif
