///////////////////////////////////////////////////////////////////////////
// Author: Flora Tasse
// Year: 2013
// Computer Laboratory-Cambridge University
///////////////////////////////////////////////////////////////////////////
#include <mepp_config.h>
#ifdef BUILD_component_HKS

#ifdef _MSC_VER
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#endif

#include "HKS_Component.h"
#include "tools/hks.h"

#include <time.h>

// subdivision
#include "../../../../mepp/Tools/Tools_sqrt3.h"
#include "../../../../mepp/viewer.hxx"

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
typedef OpenMesh::TriMesh_ArrayKernelT<> Mesh;

template <class MeshT>
void polyhedron2openmesh(PolyhedronPtr m_mesh_ptr, MeshT &oMesh)
{
    oMesh.clear();
    m_mesh_ptr->set_index_vertices();

    Vertex_iterator pVertex	=	m_mesh_ptr->vertices_begin();
    for(;pVertex != m_mesh_ptr->vertices_end();pVertex++)
    {
        Enriched_kernel::Point_3 pt = pVertex->point();
        oMesh.add_vertex(typename MeshT::Point(pt[0], pt[1], pt[2]));
    }

    Facet_iterator pFacet	=	m_mesh_ptr->facets_begin();
    for(;pFacet	!= m_mesh_ptr->facets_end();pFacet++)
    {
        std::vector<typename MeshT::VertexHandle> vhandles;
        Halfedge_around_facet_circulator pHalfedge = pFacet->facet_begin();
        do
        {
            int vid = pHalfedge->vertex()->tag();
            vhandles.push_back(oMesh.vertex_handle(vid));
        }
        while(++pHalfedge != pFacet->facet_begin());
        oMesh.add_face(vhandles);
    }
}

void initLUTIndex(double LUT[])
{
    int i=0;
    LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.515600;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.531300;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.546900;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.562500;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.578100;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.593800;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.609400;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.625000;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.640600;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.656300;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.671900;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.687500;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.703100;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.718800;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.734400;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.750000;
    LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.765600;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.781300;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.796900;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.812500;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.828100;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.843800;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.859400;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.875000;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.890600;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.906300;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.921900;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.937500;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.953100;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.968800;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.984400;		LUT[i++]=	0.000000;	LUT[i++]=	0.000000;	LUT[i++]=	1.000000;
    LUT[i++]=	0.000000;	LUT[i++]=	0.015600;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.031300;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.046900;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.062500;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.078100;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.093800;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.109400;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.125000;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.140600;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.156300;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.171900;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.187500;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.203100;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.218800;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.234400;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.250000;	LUT[i++]=	1.000000;
    LUT[i++]=	0.000000;	LUT[i++]=	0.265600;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.281300;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.296900;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.312500;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.328100;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.343800;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.359400;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.375000;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.390600;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.406300;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.421900;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.437500;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.453100;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.468800;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.484400;	LUT[i++]=	1.000000;
    LUT[i++]=	0.000000;	LUT[i++]=	0.500000;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.515600;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.531300;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.546900;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.562500;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.578100;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.593800;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.609400;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.625000;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.640600;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.656300;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.671900;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.687500;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.703100;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.718800;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.734400;	LUT[i++]=	1.000000;
    LUT[i++]=	0.000000;	LUT[i++]=	0.750000;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.765600;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.781300;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.796900;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.812500;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.828100;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.843800;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.859400;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.875000;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.890600;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.906300;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.921900;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.937500;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.953100;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.968800;	LUT[i++]=	1.000000;		LUT[i++]=	0.000000;	LUT[i++]=	0.984400;	LUT[i++]=	1.000000;
    LUT[i++]=	0.000000;	LUT[i++]=	1.000000;	LUT[i++]=	1.000000;		LUT[i++]=	0.015600;	LUT[i++]=	1.000000;	LUT[i++]=	1.000000;		LUT[i++]=	0.031300;	LUT[i++]=	1.000000;	LUT[i++]=	0.984400;		LUT[i++]=	0.046900;	LUT[i++]=	1.000000;	LUT[i++]=	0.968800;		LUT[i++]=	0.062500;	LUT[i++]=	1.000000;	LUT[i++]=	0.953100;		LUT[i++]=	0.078100;	LUT[i++]=	1.000000;	LUT[i++]=	0.937500;		LUT[i++]=	0.093800;	LUT[i++]=	1.000000;	LUT[i++]=	0.921900;		LUT[i++]=	0.109400;	LUT[i++]=	1.000000;	LUT[i++]=	0.906300;		LUT[i++]=	0.125000;	LUT[i++]=	1.000000;	LUT[i++]=	0.890600;		LUT[i++]=	0.140600;	LUT[i++]=	1.000000;	LUT[i++]=	0.875000;		LUT[i++]=	0.156300;	LUT[i++]=	1.000000;	LUT[i++]=	0.859400;		LUT[i++]=	0.171900;	LUT[i++]=	1.000000;	LUT[i++]=	0.843800;		LUT[i++]=	0.187500;	LUT[i++]=	1.000000;	LUT[i++]=	0.828100;		LUT[i++]=	0.203100;	LUT[i++]=	1.000000;	LUT[i++]=	0.812500;		LUT[i++]=	0.218800;	LUT[i++]=	1.000000;	LUT[i++]=	0.796900;		LUT[i++]=	0.234400;	LUT[i++]=	1.000000;	LUT[i++]=	0.781300;
    LUT[i++]=	0.250000;	LUT[i++]=	1.000000;	LUT[i++]=	0.765600;		LUT[i++]=	0.265600;	LUT[i++]=	1.000000;	LUT[i++]=	0.750000;		LUT[i++]=	0.281300;	LUT[i++]=	1.000000;	LUT[i++]=	0.734400;		LUT[i++]=	0.296900;	LUT[i++]=	1.000000;	LUT[i++]=	0.718800;		LUT[i++]=	0.312500;	LUT[i++]=	1.000000;	LUT[i++]=	0.703100;		LUT[i++]=	0.328100;	LUT[i++]=	1.000000;	LUT[i++]=	0.687500;		LUT[i++]=	0.343800;	LUT[i++]=	1.000000;	LUT[i++]=	0.671900;		LUT[i++]=	0.359400;	LUT[i++]=	1.000000;	LUT[i++]=	0.656300;		LUT[i++]=	0.375000;	LUT[i++]=	1.000000;	LUT[i++]=	0.640600;		LUT[i++]=	0.390600;	LUT[i++]=	1.000000;	LUT[i++]=	0.625000;		LUT[i++]=	0.406300;	LUT[i++]=	1.000000;	LUT[i++]=	0.609400;		LUT[i++]=	0.421900;	LUT[i++]=	1.000000;	LUT[i++]=	0.593800;		LUT[i++]=	0.437500;	LUT[i++]=	1.000000;	LUT[i++]=	0.578100;		LUT[i++]=	0.453100;	LUT[i++]=	1.000000;	LUT[i++]=	0.562500;		LUT[i++]=	0.468800;	LUT[i++]=	1.000000;	LUT[i++]=	0.546900;		LUT[i++]=	0.484400;	LUT[i++]=	1.000000;	LUT[i++]=	0.531300;
    LUT[i++]=	0.500000;	LUT[i++]=	1.000000;	LUT[i++]=	0.515600;		LUT[i++]=	0.515600;	LUT[i++]=	1.000000;	LUT[i++]=	0.500000;		LUT[i++]=	0.531300;	LUT[i++]=	1.000000;	LUT[i++]=	0.484400;		LUT[i++]=	0.546900;	LUT[i++]=	1.000000;	LUT[i++]=	0.468800;		LUT[i++]=	0.562500;	LUT[i++]=	1.000000;	LUT[i++]=	0.453100;		LUT[i++]=	0.578100;	LUT[i++]=	1.000000;	LUT[i++]=	0.437500;		LUT[i++]=	0.593800;	LUT[i++]=	1.000000;	LUT[i++]=	0.421900;		LUT[i++]=	0.609400;	LUT[i++]=	1.000000;	LUT[i++]=	0.406300;		LUT[i++]=	0.625000;	LUT[i++]=	1.000000;	LUT[i++]=	0.390600;		LUT[i++]=	0.640600;	LUT[i++]=	1.000000;	LUT[i++]=	0.375000;		LUT[i++]=	0.656300;	LUT[i++]=	1.000000;	LUT[i++]=	0.359400;		LUT[i++]=	0.671900;	LUT[i++]=	1.000000;	LUT[i++]=	0.343800;		LUT[i++]=	0.687500;	LUT[i++]=	1.000000;	LUT[i++]=	0.328100;		LUT[i++]=	0.703100;	LUT[i++]=	1.000000;	LUT[i++]=	0.312500;		LUT[i++]=	0.718800;	LUT[i++]=	1.000000;	LUT[i++]=	0.296900;		LUT[i++]=	0.734400;	LUT[i++]=	1.000000;	LUT[i++]=	0.281300;
    LUT[i++]=	0.750000;	LUT[i++]=	1.000000;	LUT[i++]=	0.265600;		LUT[i++]=	0.765600;	LUT[i++]=	1.000000;	LUT[i++]=	0.250000;		LUT[i++]=	0.781300;	LUT[i++]=	1.000000;	LUT[i++]=	0.234400;		LUT[i++]=	0.796900;	LUT[i++]=	1.000000;	LUT[i++]=	0.218800;		LUT[i++]=	0.812500;	LUT[i++]=	1.000000;	LUT[i++]=	0.203100;		LUT[i++]=	0.828100;	LUT[i++]=	1.000000;	LUT[i++]=	0.187500;		LUT[i++]=	0.843800;	LUT[i++]=	1.000000;	LUT[i++]=	0.171900;		LUT[i++]=	0.859400;	LUT[i++]=	1.000000;	LUT[i++]=	0.156300;		LUT[i++]=	0.875000;	LUT[i++]=	1.000000;	LUT[i++]=	0.140600;		LUT[i++]=	0.890600;	LUT[i++]=	1.000000;	LUT[i++]=	0.125000;		LUT[i++]=	0.906300;	LUT[i++]=	1.000000;	LUT[i++]=	0.109400;		LUT[i++]=	0.921900;	LUT[i++]=	1.000000;	LUT[i++]=	0.093800;		LUT[i++]=	0.937500;	LUT[i++]=	1.000000;	LUT[i++]=	0.078100;		LUT[i++]=	0.953100;	LUT[i++]=	1.000000;	LUT[i++]=	0.062500;		LUT[i++]=	0.968800;	LUT[i++]=	1.000000;	LUT[i++]=	0.046900;		LUT[i++]=	0.984400;	LUT[i++]=	1.000000;	LUT[i++]=	0.031300;
    LUT[i++]=	1.000000;	LUT[i++]=	1.000000;	LUT[i++]=	0.015600;		LUT[i++]=	1.000000;	LUT[i++]=	1.000000;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.984400;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.968800;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.953100;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.937500;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.921900;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.906300;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.890600;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.875000;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.859400;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.843800;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.828100;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.812500;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.796900;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.781300;	LUT[i++]=	0.000000;
    LUT[i++]=	1.000000;	LUT[i++]=	0.765600;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.750000;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.734400;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.718800;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.703100;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.687500;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.671900;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.656300;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.640600;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.625000;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.609400;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.593800;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.578100;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.562500;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.546900;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.531300;	LUT[i++]=	0.000000;
    LUT[i++]=	1.000000;	LUT[i++]=	0.515600;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.500000;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.484400;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.468800;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.453100;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.437500;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.421900;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.406300;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.390600;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.375000;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.359400;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.343800;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.328100;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.312500;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.296900;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.281300;	LUT[i++]=	0.000000;
    LUT[i++]=	1.000000;	LUT[i++]=	0.265600;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.250000;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.234400;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.218800;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.203100;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.187500;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.171900;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.156300;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.140600;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.125000;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.109400;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.093800;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.078100;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.062500;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.046900;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.031300;	LUT[i++]=	0.000000;
    LUT[i++]=	1.000000;	LUT[i++]=	0.015600;	LUT[i++]=	0.000000;		LUT[i++]=	1.000000;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.984400;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.968800;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.953100;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.937500;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.921900;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.906300;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.890600;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.875000;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.859400;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.843800;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.828100;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.812500;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.796900;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.781300;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;
    LUT[i++]=	0.765600;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.750000;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.734400;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.718800;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.703100;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.687500;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.671900;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.656300;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.640600;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.625000;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.609400;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.593800;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.578100;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.562500;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.546900;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;		LUT[i++]=	0.531300;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;
    LUT[i++]=	0.515600;	LUT[i++]=	0.000000;	LUT[i++]=	0.000000;
}

HKS_Component::HKS_Component(Viewer* v, PolyhedronPtr p):mepp_component(v, p)
{
    initLUTIndex(LUT);
    m_mesh_ptr = v->getScenePtr()->get_polyhedron();

    nev = 300;
    nsteps = 101;
    useGeodesic = false;
    eig_id = hks_tid = 0;

    // MEPP 2
    componentName = "HKS_Component";
    init = 1;
}

void HKS_Component::computeSpectra()
{
    Mesh mesh;
    polyhedron2openmesh(m_mesh_ptr, mesh);

    Eigen::VectorXd evals;
    Eigen::MatrixXd evecs;
    nev = std::min(nev, (int)m_mesh_ptr->size_of_vertices());

    compute_spectra(mesh, evals, evecs, nev);
    m_mesh_ptr->evals.resize(evals.rows());
    for (int i=0; i<evals.rows(); ++i)  m_mesh_ptr->evals[i] = evals(i);

    int i=0;
    Vertex_iterator pVertex	=	m_mesh_ptr->vertices_begin();
    for(;pVertex != m_mesh_ptr->vertices_end();pVertex++)
    {
        pVertex->evecs.resize(evals.rows());
        for (int j=0; j<evals.rows(); ++j)  pVertex->evecs[j] = evecs(i, j);
        ++i;
    }
}

void HKS_Component::computeHKS()
{
    Eigen::VectorXd evals;
    Eigen::MatrixXd evecs, hks;
    nsteps = std::min(nsteps, (int)m_mesh_ptr->evals.size()-1);

    evals.resize(m_mesh_ptr->evals.size());
    for (int i=0; i<evals.rows(); ++i) evals(i) = m_mesh_ptr->evals[i];

    int i=0;
    evecs.resize(m_mesh_ptr->size_of_facets(), evals.rows());
    Vertex_iterator pVertex	=	m_mesh_ptr->vertices_begin();
    for(;pVertex != m_mesh_ptr->vertices_end();pVertex++)
    {
        for (int j=0; j<evals.rows(); ++j)
            evecs(i, j) = pVertex->evecs[j];
        ++i;
    }

    hks = compute_hks(evals, evecs, nsteps-1);

    i=0;
    pVertex	=	m_mesh_ptr->vertices_begin();
    for(;pVertex != m_mesh_ptr->vertices_end();pVertex++)
    {
        pVertex->hks.resize(hks.cols());
        for (int j=0; j<hks.cols(); ++j)  pVertex->hks[j] = hks(i, j);
        ++i;
    }
}

bool HKS_Component::saveHKS(std::string path)
{
    return false;
}

bool HKS_Component::loadHKS(std::string path)
{
    return false;
}

void HKS_Component::hksToColorMap()
{
    double minv = DBL_MAX, maxv = DBL_MIN;

    for (Vertex_iterator pVertex = m_mesh_ptr->vertices_begin();
         pVertex != m_mesh_ptr->vertices_end();
         pVertex++)
    {
        minv = std::min(pVertex->hks[hks_tid], minv);
        maxv = std::max(pVertex->hks[hks_tid], maxv);
    }

    for (Vertex_iterator pVertex = pVertex = m_mesh_ptr->vertices_begin();
         pVertex != m_mesh_ptr->vertices_end();
         pVertex++)
    {

        double R=(pVertex->hks[hks_tid]-minv)/(maxv-minv)*255;
        int indiceLut=floor(std::min(R, 255.0));
        pVertex->color(LUT[3*indiceLut],
                LUT[3*indiceLut+1],
                LUT[3*indiceLut+2]);
    }
}

void HKS_Component::spectraToColorMap()
{
    double minv = DBL_MAX, maxv = DBL_MIN;

    for (Vertex_iterator pVertex = m_mesh_ptr->vertices_begin();
         pVertex != m_mesh_ptr->vertices_end();
         pVertex++)
    {
        minv = std::min(pVertex->evecs[eig_id], minv);
        maxv = std::max(pVertex->evecs[eig_id], maxv);
    }

    for (Vertex_iterator pVertex = pVertex = m_mesh_ptr->vertices_begin();
         pVertex != m_mesh_ptr->vertices_end();
         pVertex++)
    {

        double R=(pVertex->evecs[eig_id]-minv)/(maxv-minv)*255;
        int indiceLut=floor(std::min(R, 255.0));
        pVertex->color(LUT[3*indiceLut],
                LUT[3*indiceLut+1],
                LUT[3*indiceLut+2]);
    }
}

#endif
