#ifndef Partition_ITEMS_H
#define Partition_ITEMS_H

#include <mepp_config.h>
#ifdef BUILD_component_Partition

#include "../../../../mepp/Polyhedron/polyhedron_shared_items.h"

/*!
 * \class Partition_Facet
 * \brief Enriches the Facets of a Polyhedra
 */
 template <class Refs, class T, class P, class Norm, class Plane>
class Partition_Facet : virtual public MEPP_Common_Facet<Refs, T, Norm>
{
	public:

        Partition_Facet() { LabelSeg = 0; }
        int LabelSeg;
		
		};


/*!
 * \class Partition_Halfedge
 * \brief Enriches the Halfedges of a Polyhedra
 */
template <class Refs, class Tprev, class Tvertex, class Tface, class Norm>
class Partition_Halfedge : virtual public MEPP_Common_Halfedge<Refs,Tprev,Tvertex,Tface,Norm>
{
	public:

        Partition_Halfedge() {}
};


/*!
 * \class Partition_Vertex
 * \brief Enriches the Vertices of a Polyhedra
 */
template <class Refs, class T, class P, class Norm>
class Partition_Vertex : virtual public MEPP_Common_Vertex<Refs, T, P, Norm>
{
	public:

        Partition_Vertex() {}
};


/*!
 * \class Partition_Polyhedron
 * \brief Enriches the Polyhedron
 */
template <class kernel, class items>
class Partition_Polyhedron : virtual public MEPP_Common_Polyhedron<kernel,items>
{
	public:

        Partition_Polyhedron() {}
        std::vector<std::string> labelTitles;
};

#endif

#endif // Partition_ITEMS_H
