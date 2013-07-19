#ifndef Skeleton_Extraction_ITEMS_H
#define Skeleton_Extraction_ITEMS_H

#include <mepp_config.h>
#ifdef BUILD_component_Skeleton_Extraction

#include "../../../../mepp/Polyhedron/polyhedron_shared_items.h"

/*!
 * \class Skeleton_Extraction_Facet
 * \brief Enriches the Facets of a Polyhedra
 */
 template <class Refs, class T, class P, class Norm, class Plane>
class Skeleton_Extraction_Facet : virtual public MEPP_Common_Facet<Refs, T, Norm>
{
	public:

        Skeleton_Extraction_Facet() {}
		
		};


/*!
 * \class Skeleton_Extraction_Halfedge
 * \brief Enriches the Halfedges of a Polyhedra
 */
template <class Refs, class Tprev, class Tvertex, class Tface, class Norm>
class Skeleton_Extraction_Halfedge : virtual public MEPP_Common_Halfedge<Refs,Tprev,Tvertex,Tface,Norm>
{
	public:

        Skeleton_Extraction_Halfedge() {}
};


/*!
 * \class Skeleton_Extraction_Vertex
 * \brief Enriches the Vertices of a Polyhedra
 */
template <class Refs, class T, class P, class Norm>
class Skeleton_Extraction_Vertex : virtual public MEPP_Common_Vertex<Refs, T, P, Norm>
{
	public:

        Skeleton_Extraction_Vertex() {
            m_thickness = 0.0;
        }

        double& thickness() {return m_thickness;}

    private:
        double m_thickness;
};


/*!
 * \class Skeleton_Extraction_Polyhedron
 * \brief Enriches the Polyhedron
 */
template <class kernel, class items>
class Skeleton_Extraction_Polyhedron : virtual public MEPP_Common_Polyhedron<kernel,items>
{
	public:

        Skeleton_Extraction_Polyhedron() {}
};

#endif

#endif // Skeleton_Extraction_ITEMS_H
