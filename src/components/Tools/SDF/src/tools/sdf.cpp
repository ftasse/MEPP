#include <fstream>
#include <omp.h>
#include <CGAL/centroid.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/AABB_polyhedron_triangle_primitive.h>

#include "sdf.h"

#define EPS 1e-5

typedef Polyhedron::Facet_iterator Facet_iterator;
typedef Polyhedron::Halfedge_around_facet_circulator Halfedge_facet_circulator;

typedef CGAL::AABB_polyhedron_triangle_primitive<Kernel,Polyhedron> Primitive;
typedef CGAL::AABB_traits<Kernel, Primitive> Traits;
typedef CGAL::AABB_tree<Traits> Tree;
typedef Tree::Object_and_primitive_id Object_and_primitive_id;
typedef Tree::Primitive_id Primitive_id;

Kernel::Point_3 compute_centroid(Facet_iterator fit);
std::vector<Kernel::Ray_3> cone_rays(Kernel::Point_3 p, Kernel::Ray_3 ctr_ray, double cone_ang, int nb_cone_rays);
double compute_sdf(Kernel::Point_3 ctr, std::vector<Kernel::Ray_3> &rays, Tree& tree);

std::vector<double> compute_face_sdf(Polyhedron &P,
                                     std::vector<double> &sdf_values,
                                     double cone_ang,
                                     int nb_cone_rays,
                                     int nb_smoothing_iter)
{
    std::for_each( P.facets_begin(),   P.facets_end(),   Facet_normal());
    std::for_each( P.vertices_begin(), P.vertices_end(), Vertex_normal());
    sdf_values.resize(P.size_of_facets(), 0.0);

    // constructs AABB tree
    Tree tree(P.facets_begin(),P.facets_end());

    unsigned int i = 0;
    //#pragma omp parallel for
    for ( Facet_iterator fit = P.facets_begin(); fit != P.facets_end(); ++fit)
    {
        fit->id()  = i;
        Kernel::Point_3 ctr = compute_centroid(fit);
        Kernel::Ray_3 cone_ray(ctr - EPS*fit->normal(), -fit->normal());

        std::vector<Kernel::Ray_3> rays = cone_rays(ctr, cone_ray, cone_ang, nb_cone_rays);
        sdf_values[i] = compute_sdf(ctr, rays, tree);

        ++i;
    }

    double min_sdf = DBL_MAX;
    double max_sdf = DBL_MIN;
    for (Facet_iterator fit = P.facets_begin(); fit != P.facets_end(); ++fit)
    {
        if (sdf_values[fit->id()] < EPS)
        {
            double val = 0.0;
            int count = 0;
            Halfedge_facet_circulator hc = fit->facet_begin();
            do {
                if (!hc->is_border_edge())
                {
                    Polyhedron::Facet_handle fc = hc->opposite()->facet();
                    if (sdf_values[fc->id()] > EPS)
                    {
                        val += 0.0;
                        ++count;
                    }
                }
            } while ( ++hc != fit->facet_begin());
            if (count > 0)
            {
                sdf_values[fit->id()] = val/count;
            }
        }
        min_sdf = std::min(sdf_values[fit->id()], min_sdf);
        max_sdf = std::max(sdf_values[fit->id()], max_sdf);
    }

    //Smoothing
    double diff_thres = (max_sdf-min_sdf)*50/100.0;
    std::vector<double> org_values(sdf_values.begin(), sdf_values.end());
    std::vector<double> tmp_values(sdf_values.begin(), sdf_values.end());
    for (unsigned int k=0; k<nb_smoothing_iter; ++k)
    {
        for (Facet_iterator fit = P.facets_begin(); fit != P.facets_end(); ++fit)
        {
            double val = sdf_values[fit->id()];
            double tot_w = 1.0;

            Halfedge_facet_circulator hc = fit->facet_begin();
            do {
                if (!hc->is_border_edge())
                {
                    Polyhedron::Facet_handle fc = hc->opposite()->facet();
                    //if (std::abs(sdf_values[fit->id()] - org_values[fc->id()]) <= diff_thres)
                    {
                        double w = 1.0; //Should be the geodesic distance-based gaussian function
                        tot_w += w;
                        val += w*sdf_values[fc->id()];  //org_values[fc->id()])
                    }
                }
            } while ( ++hc != fit->facet_begin());

            tmp_values[fit->id()] = val/tot_w;
        }

        for (Facet_iterator fit = P.facets_begin(); fit != P.facets_end(); ++fit)
        {
            sdf_values[fit->id()] = tmp_values[fit->id()];
            //std::cerr << k << " diff: " << sdf_values[fit->id()] - org_values[fit->id()] << "\n" << std::flush;
        }
    }

    return sdf_values;
}

std::vector<double> compute_vertex_sdf(Polyhedron &P,
                                       std::vector<double> &sdf_values,
                                       double cone_ang,
                                       int nb_cone_rays,
                                       int nb_smoothing_iter)
{

}

std::vector<double> compute_face_sdf(std::string mesh_path, double cone_ang, int nb_cone_rays, int nb_smoothing_iter)
{
    std::vector<double> sdf_values;

    Polyhedron P;
    std::ifstream ifs(mesh_path.c_str());
    if (!ifs.is_open())
        return sdf_values;
    else
        ifs >> P;
    std::cout << "Num of faces: " << P.size_of_facets() << ".\n";

    compute_face_sdf(P, sdf_values, cone_ang, nb_cone_rays, nb_smoothing_iter);
    return sdf_values;
}

std::vector<double> compute_vertex_sdf(std::string mesh_path, double cone_ang, int nb_cone_rays, int nb_smoothing_iter)
{
    std::vector<double> sdf_values;

    Polyhedron P;
    std::ifstream ifs(mesh_path.c_str());
    if (!ifs.is_open())
        return sdf_values;
    else
        ifs >> P;
    std::cout << "Num of vertices: " << P.size_of_vertices() << ".\n";

    compute_vertex_sdf(P, sdf_values, cone_ang, nb_cone_rays, nb_smoothing_iter);
    return sdf_values;
}

Kernel::Point_3 compute_centroid(Facet_iterator fit)
{
    std::vector<Kernel::Point_3> points;
    Halfedge_facet_circulator hc = fit->facet_begin();
    do {
        points.push_back(hc->vertex()->point());
    } while ( ++hc != fit->facet_begin());
    return CGAL::centroid(points.begin(), points.end());
}

Kernel::Aff_transformation_3 rot_around_vertex(Kernel::Vector_3 v, double ang_in_rads)
{
    float c = cos(ang_in_rads);
    float s = sin(ang_in_rads);
    float t = 1-c;
    return Kernel::Aff_transformation_3 (
                t*v.x()*v.x() + c, t*v.x()*v.y() + s*v.z(), t*v.x()*v.z() - s*v.y(), 0,
                t*v.x()*v.y() - s*v.z(), t*v.y()*v.y() + c, t*v.y()*v.z() + s*v.x(), 0,
                t*v.x()*v.z() + s*v.y(), t*v.y()*v.z() - s * v.x(), t*v.z()*v.z() + c, 0);
}

std::vector<Kernel::Ray_3> cone_rays(Kernel::Point_3 p, Kernel::Ray_3 ctr_ray, double cone_ang, int nb_cone_rays)
{
    int numCones = 3; //cone_ang/(10.0*M_PI/180.0);
    std::vector<Kernel::Ray_3> rays(nb_cone_rays);
    rays[0] = ctr_ray;
    int spread = (nb_cone_rays - 1)/numCones;

    int k=1;
    for (int i=0; i<numCones; i++) {
        double degree = (cone_ang * (i+1))/numCones;
        Kernel::Vector_3 vec = ctr_ray.to_vector(); vec = vec/vec.squared_length();
        Kernel::Plane_3 plane(ctr_ray.source(), vec);
        Kernel::Vector_3 base = plane.base1()/sqrt(plane.base1().squared_length());
        Kernel::Aff_transformation_3 aff = rot_around_vertex(base, degree/2);
        Kernel::Aff_transformation_3 aroundRay = rot_around_vertex(vec, 2*M_PI/spread);

        Kernel::Vector_3 spinMe = vec;
        spinMe = spinMe / sqrt(spinMe.squared_length());
        spinMe = spinMe.transform(aff);
        for (int j=0;j<spread; j++) {
            double ang_diff = std::abs(acos(vec*spinMe));
            rays[k] = Kernel::Ray_3(p + spinMe * EPS, spinMe);
            spinMe = spinMe.transform(aroundRay);
            ++k;
        }
    }

    return rays;
}

double compute_sdf(Kernel::Point_3 ctr, std::vector<Kernel::Ray_3> &rays, Tree &tree)
{
    std::vector<double> lengths, weights;
    lengths.reserve(rays.size());
    weights.reserve(rays.size());
    Kernel::Vector_3 v0 = rays[0].to_vector(); v0 = v0/v0.squared_length();

    for (unsigned int j=0; j<rays.size(); ++j)
    {
        Kernel::Ray_3 ray = rays[j];
        if(tree.do_intersect(ray))
        {
            std::list<Object_and_primitive_id> intersections;
            tree.all_intersections(ray, std::back_inserter(intersections));
            if(intersections.size() > 0)
            {
                double min_dist = DBL_MAX;
                Object_and_primitive_id op;
                for (std::list<Object_and_primitive_id>::iterator it = intersections.begin(); it!=intersections.end(); ++it)
                {
                    Kernel::Point_3 point;
                    CGAL::assign(point, it->first);
                    double dist = CGAL::squared_distance(point, ctr);
                    if (dist < min_dist)
                    {
                        min_dist = dist;
                        op = *it;
                    }
                }

                Polyhedron::Face_handle f = op.second;
                Kernel::Vector_3 normal = -f->normal();
                if ((normal*v0) >= 0.0) continue;

                // gets intersection object
                CGAL::Object object = op.first;
                Kernel::Point_3 point;
                if(CGAL::assign(point,object))
                {
                    Kernel::Vector_3 v = ray.to_vector(); v = v/v.squared_length();
                    double ang_diff = std::abs(acos(v*v0)); //in rads
                    double w = 1.0;
                    ang_diff *= (180.0/M_PI);  //std::cerr <<ang_diff << "\n" << std::flush;

                    if (ang_diff > 1.0) w = 1/ang_diff;
                    double length = sqrt(CGAL::squared_distance(point, ctr)/2.0);
                    weights.push_back(w);
                    lengths.push_back(length);
                }
            }
        }
    }

    if (lengths.size() == 0)
        return 0.0;

    double std_dev = 0.0;
    double median = 0.0;
    double tot_length = 0.0;

    for (uint i=0; i<lengths.size(); i++) {
        tot_length += lengths[i];
    }
    tot_length /= lengths.size();

    std::vector<double> tmp(lengths);
    std::nth_element(tmp.begin(), tmp.begin()+(tmp.size()/2), tmp.end());
    median = tmp[tmp.size()/2];
    for (uint i=0; i<lengths.size();i++) {
        std_dev += std::pow(lengths[i]-tot_length, 2);
    }
    std_dev = std::sqrt(std_dev/lengths.size());

    double tot_sdf = 0.0, tot_weight = 0.0;
    for (uint i=0; i<lengths.size(); i++) {
        if (std::fabs(lengths[i]-/*result*/median) <= 0.5 * std_dev) {
            tot_sdf += lengths[i] * weights[i];
            tot_weight += weights[i];
        }
    }
    if (tot_weight > EPS)
        tot_sdf = tot_sdf / tot_weight;
    return tot_sdf;
}
