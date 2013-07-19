#ifndef MESH_CONTRACTION_H
#define MESH_CONTRACTION_H

#include <float.h>
#include <iostream>
#include <sstream>
#include <utility>
#include <Eigen/Sparse>
//#include <Eigen/UmfPackSupport>
#include <Eigen/IterativeLinearSolvers>

#include <OpenMesh/Core/Geometry/MathDefs.hh>
#include <OpenMesh/Core/Utils/Property.hh>

typedef std::pair<double, double> AreaPair;
typedef Eigen::SimplicialCholesky< Eigen::SparseMatrix<double>  > LinearSolver;
//typedef Eigen::BiCGSTAB < Eigen::SparseMatrix<double>  > LinearSolver;
//typedef Eigen::UmfPackLU < Eigen::SparseMatrix<double>  > LinearSolver;

/* from OpenFlipper ACG Geometry */
template<class VectorT>
int isObtuse(const VectorT& _p0,
             const VectorT& _p1,
             const VectorT& _p2 )
{
    const double a0 = ((_p1-_p0)|(_p2-_p0));

    if ( a0<0.0) return 1;
    else
    {
        const double a1 = ((_p0-_p1)|(_p2-_p1));

        if (a1 < 0.0 ) return 2;
        else
        {
            const double a2 = ((_p0-_p2)|(_p1-_p2));

            if (a2 < 0.0 ) return 3;
            else return 0;
        }
    }
}

template <class M>
class Contraction
{
public:
    Contraction(M * _mesh, int _max_iter=20, double _eps = 1e-6, double _wh0 = 1.0, double _sl = 2.0)
        :orig_mesh(_mesh), max_iter(_max_iter), eps(_eps), wh0(_wh0), sl(_sl)
    {
        mesh = new M (*orig_mesh);
        mesh->request_vertex_normals();
        mesh->request_face_normals();
        mesh->update_normals();

        mesh->add_property(vprop_origpos);
        for (typename M::VertexIter vit = mesh->vertices_begin(); vit!=mesh->vertices_end(); ++vit)
        {
            orig_position(vit) = mesh->point(vit);
        }

        mesh->add_property(hprop_weight);
        mesh->add_property(vprop_weight);
        mesh->add_property(vprop_varea);
        mesh->add_property(vprop_degenerate);
        mesh->add_property(vprop_area);
        mesh->add_property(vprop_laplacian_weight);
        mesh->add_property(vprop_anchor_weight);
    }

    ~Contraction()
    {
        mesh->remove_property(hprop_weight);
        mesh->remove_property(vprop_weight);
        mesh->remove_property(vprop_varea);
        mesh->remove_property(vprop_degenerate);
        mesh->remove_property(vprop_area);
        mesh->remove_property(vprop_laplacian_weight);
        mesh->remove_property(vprop_anchor_weight);

        delete mesh; mesh = NULL;
        orig_mesh = NULL;
    }

    void compute_weights()
    {
        typename M::VertexIter v_it;

        for (v_it= mesh->vertices_begin(); v_it != mesh->vertices_end(); ++v_it)
        {
            typedef typename M::Scalar REALT;
            double _weight = 0.0;
            double _area = 0.0;

            typename M::ConstVertexOHalfedgeIter voh_it = mesh->cvoh_iter(v_it);
            if ( ! voh_it.handle().is_valid() ) continue;

            for(; voh_it; ++voh_it)
            {
                mesh->property(hprop_weight, voh_it.handle()) = 0.0;
                if ( mesh->is_boundary( mesh->edge_handle( voh_it.handle() ) ) )
                    continue;

                const typename M::Point p0 = mesh->point( v_it );
                const typename M::Point p1 = mesh->point( mesh->to_vertex_handle( voh_it.handle()));
                const typename M::Point p2 = mesh->point( mesh->from_vertex_handle( mesh->prev_halfedge_handle( voh_it.handle())));
                const typename M::Point p3 = mesh->point( mesh->to_vertex_handle( mesh->next_halfedge_handle( mesh->opposite_halfedge_handle(voh_it.handle()))));

                const REALT alpha = acos( OpenMesh::sane_aarg((p0-p2).normalize() | (p1-p2).normalize()) );
                const REALT beta  = acos( OpenMesh::sane_aarg((p0-p3).normalize() | (p1-p3).normalize()) );

                REALT cotw = 0.0;

                if ( !OpenMesh::is_eq(alpha,M_PI/2.0) ) cotw += (REALT(1.0))/tan(alpha);
                if ( !OpenMesh::is_eq(beta,M_PI/2.0) ) cotw += (REALT(1.0))/tan(beta);
                if ( OpenMesh::is_zero(cotw) || isinf(cotw) ) continue;

                // calculate voronoi area
                const int obt = isObtuse(p0,p1,p2);
                if(obt == 0)
                {
                    REALT gamma = acos( OpenMesh::sane_aarg((p0-p1).normalize() | (p2-p1).normalize()) );

                    REALT tmp = 0.0;
                    if ( !OpenMesh::is_eq(alpha,M_PI/2.0) ) tmp += (p0-p1).sqrnorm()*1.0/tan(alpha);
                    if ( !OpenMesh::is_eq(gamma,M_PI/2.0) ) tmp += (p0-p2).sqrnorm()*1.0/tan(gamma);
                    if ( OpenMesh::is_zero(tmp) || isinf(tmp) ) continue;
                    _area += 0.125*( tmp );
                }
                else
                {
                    if(obt == 1) _area += ((p1-p0) % (p2-p0)).norm() * 0.5 * 0.5;
                    else _area += ((p1-p0) % (p2-p0)).norm() * 0.5 * 0.25;
                }

                mesh->property(hprop_weight, voh_it.handle()) = cotw;
                _weight += cotw;

            }

            for (voh_it = mesh->cvoh_iter(v_it); voh_it; ++voh_it)
            {
                if (!OpenMesh::is_zero(_area))
                {
                    mesh->property(hprop_weight, voh_it.handle()) /= (2*_area);
                }
            }
            if (!OpenMesh::is_zero(_area))
                mesh->property(vprop_weight, v_it) = _weight/(2*_area);
            mesh->property(vprop_varea, v_it) = _area;
        }
    }

    void contraction_step()
    {
        int n = mesh->n_vertices();
        int nf = 0;
        std::vector<int> variable_indexes(n, -1);

        for (typename M::VertexIter vit = mesh->vertices_begin(); vit!=mesh->vertices_end(); ++vit)
        {
            if(is_degenerate(vit))
            {
                anchor_weight(vit) = 1e8;
                laplacian_weight(vit) = 0.0;
            }
            //if(!is_degenerate(vit))
            {
                variable_indexes[vit.handle().idx()] = nf;
                ++nf;
            }
        }

        Eigen::SparseMatrix<double> A(2*nf, n);
        Eigen::MatrixXd b = Eigen::MatrixXd::Zero(2*nf, 3);

        typedef Eigen::Triplet<double> Triplet;
        std::vector<Triplet> triplets;
        triplets.reserve(nf*7);

        for (typename M::VertexIter vit = mesh->vertices_begin(); vit!=mesh->vertices_end(); ++vit)
        {
            //if(!is_degenerate(vit))
            {
                typename M::VertexHandle vh = vit.handle();
                triplets.push_back(Triplet(variable_indexes[vh.idx()], variable_indexes[vh.idx()],
                        -weight(vh)*laplacian_weight(vh)));

                for (typename M::VertexOHalfedgeIter hit = mesh->voh_iter(vit); hit; ++hit)
                {
                    typename M::VertexHandle to = mesh->to_vertex_handle(hit.handle());
                    triplets.push_back(Triplet(variable_indexes[vh.idx()],
                                       to.idx(), weight(hit.handle())*laplacian_weight(vh)));
                    //std::cout << "A: " << vh.idx() << " " << to.idx() << " " << weight(hit.handle())*laplacian_weight(vh) << "\n" << std::flush;
                }
                //std::cout << "A: " << vh.idx() << " " << vh.idx() << " "  << -weight(vh)*laplacian_weight(vh) << "\n" << std::flush;
            }
        }

        for (typename M::VertexIter vit = mesh->vertices_begin(); vit!=mesh->vertices_end(); ++vit)
        {
            int i = vit.handle().idx();
            typename M::VertexHandle vh = vit.handle();
            //if(!is_degenerate(vit))
            {
                triplets.push_back(Triplet(nf + variable_indexes[i], i, anchor_weight(vh)));
                for (int k=0; k<3; ++k)
                    b(nf + variable_indexes[i], k) = anchor_weight(vh)*mesh->point(vit)[k];
                if (nf < 150)
                    std::cout << "b:" << variable_indexes[i] << " " << anchor_weight(vh) << " " << weight(vh) << "\n" << std::flush;
            }
        }

        A.setFromTriplets(triplets.begin(), triplets.end());
        Eigen::SparseMatrix<double>  At = A.transpose(), AtA = At*A;

        std::cout <<  "Solve equation \n" << std::flush;
        LinearSolver solver;
        solver.compute(AtA);

        if(solver.info()!=Eigen::Success)
            std::cerr << "[ERROR] Mesh Contraction: Could not compute llt factorization." << std::endl << std::flush;
        Eigen::MatrixXd x = solver.solve(At*b);
        if(solver.info()!=Eigen::Success)
            std::cerr << "[ERROR] Mesh Contraction: Could not solve linear system." << std::endl << std::flush;

        for (typename M::VertexIter vit = mesh->vertices_begin(); vit!=mesh->vertices_end(); ++vit)
        {
            if(!is_degenerate(vit))
            {
                for (int k=0; k<3; ++k)
                    mesh->point(vit)[k] = x(variable_indexes[vit.handle().idx()], k);
            }
        }
    }

    double contract(double _wl0, double _wh0, double _sl)
    {
        average_face_area = compute_average_face_area();
        if (_wl0 < eps) wl0 = sqrt(average_face_area)*1e0;
        else wl0 = _wl0;
        wh0 = _wh0;
        sl = _sl;

        double volume = compute_mesh_volume();
        double prev_volume = 0.0;
        int n_iter = 1;
        double max_contraction_squared = 1e5;
        double max_cotan_weight = 1e8;

        //Initialization
        std::cout << "\nInitial weights & volume: " << wh0 << " " << wl0 << " " << volume << std::endl << std::flush;
        for (typename M::VertexIter vit = mesh->vertices_begin(); vit!=mesh->vertices_end(); ++vit)
        {
            is_degenerate(vit) = false;
            laplacian_weight(vit) = wl0;
            anchor_weight(vit) = wh0;
        }
        compute_weights();
        update_vertex_areas(true);
        detect_degenerate_vertices(max_contraction_squared, max_cotan_weight);

        //Iteration
        while ( true )
        {
            contraction_step();
            mesh->update_normals();
            prev_volume = volume;
            volume = compute_mesh_volume();

            /*
            // Save intermediate contrations to .off files
            std::stringstream ss;
            ss << "/tmp/tmp_contracted" << n_iter << ".off";
            std::ofstream ofs(ss.str().c_str());
            ofs << "OFF\n";
            ofs << mesh->n_vertices() << " " << mesh->n_faces() << " " << mesh->n_edges() << "\n";
            for (typename M::VertexIter vit = mesh->vertices_begin(); vit!=mesh->vertices_end(); ++vit)
            {
                typename M::Point pt = mesh->point(vit);
                ofs <<pt[0] << " " << pt[1] << " " << pt[2] << "\n";
            }
            for (typename M::FaceIter fit = mesh->faces_begin(); fit!=mesh->faces_end(); ++fit)
            {
                ofs << "3";
                for (typename M::FaceVertexIter fvit = mesh->fv_iter(fit); fvit; ++fvit)
                    ofs << " " << fvit.handle().idx();
                ofs << "\n";
            }
            ofs.close();*/

            std::cout << "Iter " << n_iter << ": " << volume << " " << prev_volume-volume << "\n" << std::flush;
            if(n_iter < max_iter && (prev_volume-volume)>eps)
            {
                n_iter += 1;
                for (typename M::VertexIter vit = mesh->vertices_begin(); vit!=mesh->vertices_end(); ++vit)
                    is_degenerate(vit) = false;
                compute_weights();
                update_vertex_areas(false);
                update_anchor_weights();
                update_laplacian_weights();
                detect_degenerate_vertices(max_contraction_squared, max_cotan_weight);
            } else
                break;
        }

        std::cout << "\nNbr of iterations: " << n_iter << std::endl << std::flush;
        std::cout << "Volume: " << volume << "\tVolume diff: " << prev_volume-volume  << std::endl << std::flush;
        return volume;
    }

    M contracted_mesh()
    {
        return *mesh;
    }

    OpenMesh::VPropHandleT<typename M::Point> original_position_handle()
    {
        return vprop_origpos;
    }

protected:

    double& weight(typename M::HalfedgeHandle heh)
    {
        return mesh->property(hprop_weight, heh);
    }

    double& weight(typename M::VertexHandle vh)
    {
        return mesh->property(vprop_weight, vh);
    }

    AreaPair& area(typename M::VertexHandle vh)
    {
        return mesh->property(vprop_area, vh);
    }

    typename M::Point& orig_position(typename M::VertexHandle vh)
    {
        return mesh->property(vprop_origpos, vh);
    }

    int& is_degenerate(typename M::VertexHandle vh)
    {
        return mesh->property(vprop_degenerate, vh);
    }

    double& laplacian_weight(typename M::VertexHandle vh)
    {
        return mesh->property(vprop_laplacian_weight, vh);
    }

    double& anchor_weight(typename M::VertexHandle vh)
    {
        return mesh->property(vprop_anchor_weight, vh);
    }

    void detect_degenerate_vertices(double max_contraction_squared, double max_cotan_weight)
    {
        int counter = 0;
        double min_area = average_face_area/max_contraction_squared;
        for (typename M::VertexIter vit = mesh->vertices_begin(); vit!=mesh->vertices_end(); ++vit)
        {
            if (!is_degenerate(vit))
            {
                if (weight(vit) > max_cotan_weight)
                    is_degenerate(vit) = true;
                else
                    for (typename M::VertexOHalfedgeIter hit = mesh->voh_iter(vit); hit; ++hit)
                    {
                        double a = mesh->calc_sector_area(hit.handle()) ;
                        if (!((a <= DBL_MAX && a >= -DBL_MAX)))
                            a = 0.0;
                        if (a < min_area)
                        {
                            is_degenerate(vit) = true;
                            break;
                        }
                        else if (weight(hit.handle()) > max_cotan_weight)
                        {
                            is_degenerate(vit) = true;
                            break;
                        }
                    }
            }
            if (is_degenerate(vit)) ++counter;
        }
        std::cout << "Number of degenerate vertices: " << counter << "/" << mesh->n_vertices() << std::endl;
    }

    void update_vertex_areas( bool first_time)
    {
        for (typename M::VertexIter vit = mesh->vertices_begin(); vit!=mesh->vertices_end(); ++vit)
        {
            if (!is_degenerate(vit))
            {
                /*double a = 0.0;
                for (typename M::VertexFaceIter vf_it = mesh->vf_iter(vit); vf_it; ++vf_it)
                {
                    double tmp = mesh->calc_sector_area(mesh->halfedge_handle(vf_it.handle()));
                    if (((tmp <= DBL_MAX && tmp >= -DBL_MAX)))
                        a += tmp;
                }*/
                double a = mesh->property(vprop_varea, vit);
                area(vit).second = a;
                if (first_time)	area(vit).first = a;
            }
        }
    }

    void update_laplacian_weights()
    {
        for (typename M::VertexIter vit = mesh->vertices_begin(); vit!=mesh->vertices_end(); ++vit)
        {
            if (!is_degenerate(vit))
            {
                laplacian_weight(vit) *= sl;
                //if (laplacian_weight(vit) > (1 << 10)) laplacian_weight(vit) = 1 << 10;
            }
        }
    }

    void update_anchor_weights()
    {
        for (typename M::VertexIter vit = mesh->vertices_begin(); vit!=mesh->vertices_end(); ++vit)
        {
            if (!is_degenerate(vit))
            {
                //if (area(vit).second < 1e-8)
                anchor_weight(vit) = wh0 * sqrt(area(vit).first / area(vit).second);
            }
        }
    }

    double compute_average_face_area()
    {
        double sum = 0.0;
        for (typename M::FaceIter fit = mesh->faces_begin(); fit!=mesh->faces_end(); ++fit)
        {
            sum += mesh->calc_sector_area(mesh->halfedge_handle(fit.handle()));
        }
        return sum / mesh->n_faces();
    }

    double compute_mesh_volume()
    {
        double sum = 0.0;
        for (typename M::FaceIter f_it = mesh->faces_begin(); f_it!=mesh->faces_end(); ++f_it)
        {
            //Compute signed volume of triangle
            typename M::HalfedgeHandle heh0 = mesh->halfedge_handle(f_it);
            typename M::HalfedgeHandle heh1 = mesh->next_halfedge_handle(heh0);
            typename M::HalfedgeHandle heh2 = mesh->next_halfedge_handle(heh1);

            typename M::Normal v1(mesh->point(mesh->from_vertex_handle(heh0)));
            typename M::Normal v2(mesh->point(mesh->from_vertex_handle(heh1)));
            typename M::Normal v3(mesh->point(mesh->from_vertex_handle(heh2)));

            sum += (v1|(v2%v3)) / 6.0;
        }

        return sum;
    }

private:
    M * mesh;
    M * orig_mesh;
    int max_iter;
    double eps;
    double wl0, wh0;
    double sl;

    OpenMesh::VPropHandleT<typename M::Point> vprop_origpos;

    OpenMesh::HPropHandleT<double> hprop_weight;
    OpenMesh::VPropHandleT<double> vprop_laplacian_weight;
    OpenMesh::VPropHandleT<double> vprop_anchor_weight;
    OpenMesh::VPropHandleT<double> vprop_weight;
    OpenMesh::VPropHandleT<double> vprop_varea;
    OpenMesh::VPropHandleT<int> vprop_degenerate;
    OpenMesh::VPropHandleT<AreaPair> vprop_area;
    double average_face_area;
};

#endif //MESH_CONTRACTION_H
