#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <iostream>
#include <algorithm>

typedef CGAL::Tag_true Supports_halfedge_facet;
typedef CGAL::Tag_true Supports_halfedge_vertex;


struct Facet_normal {
    template <class Facet>
    void operator()( Facet& f) {
        typename Facet::Halfedge_handle h = f.halfedge();
        typename Facet::Normal_3 normal = CGAL::cross_product(
          h->next()->vertex()->point() - h->vertex()->point(),
          h->next()->next()->vertex()->point() - h->next()->vertex()->point());
        f.normal() = normal / std::sqrt( normal * normal);
    }
};

struct Vertex_normal {
    template <class Vertex>
    void operator()( Vertex& v) {
        typename Vertex::Normal_3 normal = CGAL::NULL_VECTOR;
        typedef typename Vertex::Halfedge_around_vertex_const_circulator Circ;
        Circ c = v.vertex_begin();
        Circ d = c;
        CGAL_For_all( c, d) {
            if ( ! c->is_border())
                normal = normal + c->facet()->normal();
        }
        v.normal() = normal / std::sqrt( normal * normal);
    }
};

template <class Refs, class T, class P, class Norm, class Id>
class My_vertex : public CGAL::HalfedgeDS_vertex_base<Refs, T, P> {
    Norm  norm;
    Id idx;
public:
    My_vertex() {} // repeat mandatory constructors
    My_vertex( const P& pt) : CGAL::HalfedgeDS_vertex_base<Refs, T, P>(pt) {}
    typedef Norm Normal_3;
    Normal_3&       normal()       { return norm; }
    const Normal_3& normal() const { return norm; }
    Id&       id()       { return idx; }
    const Id& id() const { return idx; }
};

template <class Refs, class T, class Norm, class Id>
class My_facet : public CGAL::HalfedgeDS_face_base<Refs, T> {
    Norm  norm;
    Id idx;
public:
    // no constructors to repeat, since only default constructor mandatory
    typedef Norm Normal_3;
    Normal_3&       normal()       { return norm; }
    const Normal_3& normal() const { return norm; }
    Id&       id()       { return idx; }
    const Id& id() const { return idx; }
};

struct My_items : public CGAL::Polyhedron_items_3 {
    template <class Refs, class Traits>
    struct Vertex_wrapper {
        typedef typename Traits::Point_3  Point;
        typedef typename Traits::Vector_3 Normal;
        typedef My_vertex<Refs, CGAL::Tag_true, Point, Normal, unsigned int> Vertex;
    };
    template <class Refs, class Traits>
    struct Face_wrapper {
        typedef typename Traits::Vector_3 Normal;
        typedef My_facet<Refs, CGAL::Tag_true, Normal, unsigned int> Face;
    };
};

// Tie all types together and a small main function using it.

typedef CGAL::Simple_cartesian<double>                 Kernel;
typedef CGAL::Polyhedron_3<Kernel, My_items>           Polyhedron;

#endif //POLYHEDRON_H
