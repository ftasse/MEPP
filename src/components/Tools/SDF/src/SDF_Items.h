#ifndef SDF_ITEMS_H
#define SDF_ITEMS_H

#include <mepp_config.h>
#ifdef BUILD_component_SDF

#include "../../../../mepp/Polyhedron/polyhedron_shared_items.h"

/*!
 * \class SDF_Facet
 * \brief Enriches the Facets of a Polyhedra
 */
 template <class Refs, class T, class P, class Norm, class Plane>
class SDF_Facet : virtual public MEPP_Common_Facet<Refs, T, Norm>
{
	public:

        SDF_Facet() { sdfVal = 0.0; }
        double sdfVal;
        int m_id;

        int& id()
        {
            return m_id;
        }

		};



/*!
 * \class SDF_Halfedge
 * \brief Enriches the Halfedges of a Polyhedra
 */
template <class Refs, class Tprev, class Tvertex, class Tface, class Norm>
class SDF_Halfedge : virtual public MEPP_Common_Halfedge<Refs,Tprev,Tvertex,Tface,Norm>
{
	public:

        SDF_Halfedge() {}
};


/*!
 * \class SDF_Vertex
 * \brief Enriches the Vertices of a Polyhedra
 */
template <class Refs, class T, class P, class Norm>
class SDF_Vertex : virtual public MEPP_Common_Vertex<Refs, T, P, Norm>
{
	public:

        SDF_Vertex() { sdfVal = 0.0; }

        double sdfVal;
};


/*!
 * \class SDF_Polyhedron
 * \brief Enriches the Polyhedron
 */
template <class kernel, class items>
class SDF_Polyhedron : virtual public MEPP_Common_Polyhedron<kernel,items>
{
	public:

        SDF_Polyhedron() {}
};

#endif

#endif // SDF_ITEMS_H
