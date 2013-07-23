#include <iostream>
#include <fstream>
#include <sstream>
#include <OpenMesh/Core/IO/MeshIO.hh>

#include "mesh_contraction.h"
#include "mesh_simplification.h"
#include "mesh2skeleton.h"
#include "skeleton_extraction.h"
#include "skeleton.h"

using namespace skeletonExtraction;

/* Use Geomview OOGL file format. (SKEL format for skeleton)*/
std::set<int> neighbour2record(OMesh& contracted_mesh, std::vector<int>& record);
double total_length_of_adjacent_edges(OMesh& mesh,  OMesh& contracted_mesh,
                                      std::vector<int>& boundary, int idx);

namespace skeletonExtraction
{

void contractMesh(OMesh &mesh, OMesh &contracted,
                  double wl0, double wh0, double sl, int maxiter)
{
    if (mesh.n_faces() == 0)
    {
        contracted = mesh;
        return;
    }
    Contraction<OMesh> contraction(&mesh, maxiter, 1e-5);
    contraction.contract(wl0, wh0, sl);
    contracted = contraction.contracted_mesh();
}

void simplifyToSkeleton(OMesh &mesh, OMesh &contracted, Skeleton &skeleton,
                        double wShapeCost, double wSamplingCost)
{
    MeshToSkeleton mesh2skel = simplify(contracted, wShapeCost, wSamplingCost, false); //skel.fix_topology();

    skeleton = Skeleton();
    int nv = mesh2skel.num_valid_vertices();
    std::set<std::pair<int, int> > edges;
    for (MeshToSkeleton::HEdgeIter hit = mesh2skel.hedges_begin(); hit != mesh2skel.hedges_end(); ++hit)
    {
        if (hit->is_deleted) continue;
        int v1 = hit->verts[0];
        int v2 = hit->verts[1];
        if (v1 < v2)
            edges.insert(std::pair<int, int>(v1, v2));
        else
            edges.insert(std::pair<int, int>(v2, v1));
    }

    skeleton.verts.resize(nv);
    skeleton.regions.resize(nv);
    skeleton.edges.resize(edges.size());

    std::map<int, int> index_map;
    int k = 0;
    for (MeshToSkeleton::VertexIter vit = mesh2skel.vertices_begin(); vit!=mesh2skel.vertices_end(); ++vit)
    {
        if  (vit->is_deleted)   continue;
        index_map[vit->idx] = k;

        skeleton.verts[k] = Skeleton::Vec3(vit->point[0], vit->point[1], vit->point[2]);
        skeleton.regions[k].resize(vit->record.size());
        for (unsigned int i=0; i< vit->record.size(); ++i)
        {
            skeleton.regions[k][i] = vit->record[i];
        }
        ++k;
    }

    k = 0;
    for (std::set<std::pair<int, int> > ::iterator it = edges.begin(); it!=edges.end(); ++it)
    {
        skeleton.edges[k] = std::pair<int, int>(index_map[(*it).first], index_map[(*it).second]);
        ++k;
    }
}

void refineEmbedding(OMesh &mesh, OMesh &contracted, Skeleton &skeleton,
                     bool useCentroids)
{
    if (useCentroids)
    {
        for (unsigned int i=0; i<skeleton.verts.size(); ++i)
        {
            OMesh::Point ctr(0.0, 0.0, 0.0);
            for (unsigned int j=0; j< skeleton.regions[i].size(); ++j)
            {
                OMesh::Point pt = mesh.point(mesh.vertex_handle(skeleton.regions[i][j]));
                ctr += pt;
            }
            ctr /= skeleton.regions[i].size();
            for (int k=0; k<3; ++k) skeleton.verts[i][k] = ctr[k];
        }
        return;
    }

    std::vector< std::set<int> > adjacency;
    skeleton.compute_adjacency(adjacency);

    for (unsigned int i=0; i<skeleton.verts.size(); ++i)
    {
        std::vector< std::vector<int> > boundaries;
        std::vector<int> record = skeleton.regions[i];
        std::set<int> recordset = std::set<int>(record.begin(), record.end());

        if (adjacency[i].size() == 1)	//End node
        {
            boundaries.push_back(record);
        } else
        {
            std::set<int>::iterator neigh_it;
            for (neigh_it = adjacency[i].begin(); neigh_it!=adjacency[i].end(); ++neigh_it)
            {
                int j = *neigh_it;
                std::vector<int> recj = skeleton.regions[j];
                std::set<int> neigh2rec = neighbour2record(contracted, recj);

                std::vector<int>  bd;	//Get intersection of this neighbour region with that of the record of vertex vit
                std::set_intersection(recordset.begin(), recordset.end(),
                                      neigh2rec.begin(), neigh2rec.end(), std::back_inserter(bd));;

                boundaries.push_back(bd);
            }
        }

        OMesh::Point d(0.0, 0.0, 0.0);
        int nbds = boundaries.size();
        std::vector<OMesh::Point> ds(nbds, OMesh::Point(0.0, 0.0, 0.0));
        std::vector<double> ls(nbds, 0.0);
        for (int j=0; j<nbds; ++j)
        {
            for (unsigned int i=0; i< boundaries[j].size(); ++i)
            {
                double ltmp = total_length_of_adjacent_edges(mesh, contracted, boundaries[j],  boundaries[j][i]);
                ds[j] += OMesh::Scalar(ltmp)*(contracted.point(mesh.vertex_handle(boundaries[j][i])) -
                                              mesh.point(mesh.vertex_handle(boundaries[j][i])));
                ls[j] += ltmp;
            }
            if (ls[j] > 1e-8)	ds[j]/= ls[j];
            d += ds[j];
        }

        //if (nbds==2)
        OMesh::Point unrefined; for (int k=0; k<3; ++k) unrefined[k] = skeleton.verts[i][k];
        OMesh::Point refined = unrefined - (OMesh::Point(d)/OMesh::Scalar(nbds));
        for (int k=0; k<3; ++k) skeleton.verts[i][k] = refined[k];
    }
}

void extractSkeleton(OMesh &mesh, OMesh &contracted, Skeleton &skeleton,
                     double wl0, double wh0, double sl, int maxiter,
                     double wShapeCost, double wSamplingCost,
                     bool useCentroids, bool save_intermediate)
{
    contractMesh(mesh, contracted, wl0, wh0, sl, maxiter);
    if (save_intermediate) OpenMesh::IO::write_mesh(contracted, "tmp_contracted_mesh.off");

    simplifyToSkeleton(mesh, contracted, skeleton, wShapeCost, wSamplingCost);
    if (save_intermediate) skeleton.save("tmp_unrefined.skel");

    refineEmbedding(mesh, contracted, skeleton, useCentroids);
    if (save_intermediate) OpenMesh::IO::write_mesh(contracted, "tmp_contracted_mesh.off");
}

}

bool extractSkeleton(std::string mesh_path, std::string skeleton_path, bool save_intermediate)
{
    OMesh mesh;
    if (!OpenMesh::IO::read_mesh(mesh, mesh_path.c_str()))
    {
        std::cout << "Could not open mesh at path: " << mesh_path << std::endl << std::flush;
        return false;
    }
    std::cout << "Extract skeleton from " << mesh_path << "\n" << std::flush;

    OMesh contracted;
    Skeleton skeleton;
    extractSkeleton(mesh, contracted, skeleton);

    skeleton.save(skeleton_path);
    std::cout << "Skeleton successfully extracted!" << std::endl << std::flush;
    return true;
}

std::set<int> neighbour2record(OMesh& contracted_mesh, std::vector<int>& record)
{
    std::set<int> neigh2r;
    for (unsigned int i=0; i<record.size(); ++i)
    {
        OMesh::VertexHandle vh = contracted_mesh.vertex_handle(record[i]);
        for (OMesh::VertexVertexIter vvit = contracted_mesh.vv_iter(vh); vvit; ++vvit)
        {
            neigh2r.insert(vvit.handle().idx());
        }
    }
    return neigh2r;
}

double total_length_of_adjacent_edges(OMesh& mesh,  OMesh& contracted_mesh,
                                      std::vector<int>& boundary, int idx)
{
    OMesh::VertexHandle vh = contracted_mesh.vertex_handle(idx);
    OMesh::Point pt = mesh.point(mesh.vertex_handle(idx));
    std::set<int> adjacent_vids;
    std::set<int> bdary = std::set<int>(boundary.begin(), boundary.end());
    for (OMesh::VertexVertexIter vvit = contracted_mesh.vv_iter(vh); vvit; ++vvit)
    {
        adjacent_vids.insert(vvit.handle().idx());
    }

    std::vector<int> adjacents;
    std::set_intersection(adjacent_vids.begin(), adjacent_vids.end(),
                          bdary.begin(), bdary.end(), std::back_inserter(adjacents));

    double tot_l = 0.0;
    for (unsigned int k = 0; k<adjacents.size(); ++k)
    {
        OMesh::Point pt2 = mesh.point(mesh.vertex_handle(adjacents[k]));
        tot_l += (pt-pt2).norm();
    }
    return tot_l;
}
