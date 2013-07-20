#ifndef HKS_ITEMS_H
#define HKS_ITEMS_H

#include <mepp_config.h>
#ifdef BUILD_component_HKS

#include "../../../../mepp/Polyhedron/polyhedron_shared_items.h"

/*!
 * \class HKS_Facet
 * \brief Enriches the Facets of a Polyhedra
 */
 template <class Refs, class T, class P, class Norm, class Plane>
class HKS_Facet : virtual public MEPP_Common_Facet<Refs, T, Norm>
{
	public:

        HKS_Facet() {}
		
		};


/*!
 * \class HKS_Halfedge
 * \brief Enriches the Halfedges of a Polyhedra
 */
template <class Refs, class Tprev, class Tvertex, class Tface, class Norm>
class HKS_Halfedge : virtual public MEPP_Common_Halfedge<Refs,Tprev,Tvertex,Tface,Norm>
{
	public:

        HKS_Halfedge() {}
};


/*!
 * \class HKS_Vertex
 * \brief Enriches the Vertices of a Polyhedra
 */
template <class Refs, class T, class P, class Norm>
class HKS_Vertex : virtual public MEPP_Common_Vertex<Refs, T, P, Norm>
{
	public:

        HKS_Vertex() { }

        std::vector<double> evecs, hks;
};


/*!
 * \class HKS_Polyhedron
 * \brief Enriches the Polyhedron
 */
template <class kernel, class items>
class HKS_Polyhedron : virtual public MEPP_Common_Polyhedron<kernel,items>
{
	public:

        HKS_Polyhedron() {  }

        std::vector<double> evals;
};

#endif

#endif // HKS_ITEMS_H
