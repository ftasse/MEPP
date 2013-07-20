///////////////////////////////////////////////////////////////////////////
// Author: Flora Tasse
// Year: 2013
// Computer Laboratory-Cambridge University
///////////////////////////////////////////////////////////////////////////
#include <mepp_config.h>
#ifdef BUILD_component_Partition

#ifdef _MSC_VER
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#endif

#include <OpenMesh/Core/IO/MeshIO.hh>
#include "tools/segmentation.h"
#include "tools/submeshes.h"
#include "Partition_Component.h"
#include "../../../../components/Tools/HKS/src/tools/eigendecomposition.h"
#include "../../../../components/Tools/HKS/src/tools/generate_lpmatrix.h"

#include <time.h>

// subdivision
#include "../../../../mepp/Tools/Tools_sqrt3.h"
#include "../../../../mepp/viewer.hxx"

template <class MeshT>
void polyhedron2openmesh(PolyhedronPtr pMesh, MeshT &oMesh)
{
    oMesh.clear();
    pMesh->set_index_vertices();

    Vertex_iterator pVertex	=	pMesh->vertices_begin();
    for(;pVertex != pMesh->vertices_end();pVertex++)
    {
        Enriched_kernel::Point_3 pt = pVertex->point();
        oMesh.add_vertex(typename MeshT::Point(pt[0], pt[1], pt[2]));
    }

    Facet_iterator pFacet	=	pMesh->facets_begin();
    for(;pFacet	!= pMesh->facets_end();pFacet++)
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

Partition_Component::Partition_Component(Viewer* v, PolyhedronPtr p):mepp_component(v, p)
{
    initLUTIndex(LUT);
    m_mesh_ptr = v->getScenePtr()->get_polyhedron();

    numInitLabels = 5;
    isRecursive = true;
    isHierarchical  = false;
    lambda = 15;
    alpha = 1.0;
    meshProperty = LPEigProperty;

    // MEPP 2
    componentName = "Partition_Component";
    init = 1;
}

void Partition_Component::segment()
{
    Mesh mesh;
    std::vector< std::set<int> > labelling;
    polyhedron2openmesh(m_mesh_ptr, mesh);

    if (meshProperty == SDFProperty)
    {
        std::vector<double> weights(m_mesh_ptr->size_of_facets());
        int k=0;
        Facet_iterator pFacet	=	m_mesh_ptr->facets_begin();
        for(;pFacet	!= m_mesh_ptr->facets_end();pFacet++)
        {
            weights[k] = pFacet->sdfVal;
            ++k;
        }

        GMMFit gmmfit(weights, numInitLabels);
        segmentation(mesh, gmmfit, labelling, isRecursive, isHierarchical, lambda, alpha);
    } else if (meshProperty == LPEigProperty)
    {
        int nev = 2;
        Eigen::VectorXd evals;
        Eigen::MatrixXd evecs;
        Eigen::SparseMatrix<double> Wdual;

        compute_dual_laplacian_matrix_Tutte(mesh, Wdual);
        compute_eigen_decomp_arpack(Wdual, evals, evecs, nev);

        std::vector< std::vector<double> > weights(m_mesh_ptr->size_of_facets());

        //std::cout << evecs << "\n" <<  std::flush;
        //std::cout << "evals:\n " << evals << "\n" <<  std::flush;
        /*int c = nev-1, i=0;
        double minv = evecs.col(c).minCoeff();
        double maxv = evecs.col(c).maxCoeff();
        Facet_iterator pFacet	=	m_mesh_ptr->facets_begin();
        for(;pFacet	!= m_mesh_ptr->facets_end();pFacet++)
        {
            double R = (evecs.col(c)[i] - minv)*255./(maxv-minv);
            int indiceLut=floor(R);
            pFacet->color(LUT[3*indiceLut],
                    LUT[3*indiceLut+1],
                    LUT[3*indiceLut+2]);
            ++i;
        }*/

        for (int i=0; i<evecs.rows(); ++i)
        {
            for (int j=1; j<evecs.cols(); ++j)
                weights[i].push_back(evecs(i, j));
        }

        GMMFit gmmfit(weights, numInitLabels);
        segmentation(mesh, gmmfit, labelling, isRecursive, isHierarchical, lambda, alpha);
    }

    std::vector<int> labels(m_mesh_ptr->size_of_facets());
    m_mesh_ptr->labelTitles.resize(labelling.size());

    for (unsigned int i=0; i< labelling.size(); ++i)
    {
        std::stringstream ss; ss << i;
        m_mesh_ptr->labelTitles[i] = ss.str();
        for (std::set<int>::iterator it = labelling[i].begin(); it!=labelling[i].end(); ++it)
        {
            labels[*it] = i;
        }
    }

    int k=0;
    Facet_iterator pFacet	=	m_mesh_ptr->facets_begin();
    for(;pFacet	!= m_mesh_ptr->facets_end();pFacet++)
    {
        pFacet->LabelSeg = labels[k];
        ++k;
    }
}

bool Partition_Component::saveLabelling(std::string path)
{
    std::ofstream ofs(path.c_str());
    if (!ofs.is_open())
        return false;

    Facet_iterator pFacet	=	m_mesh_ptr->facets_begin();
    for(;pFacet	!= m_mesh_ptr->facets_end();pFacet++)
    {
        ofs << pFacet->LabelSeg << "\n";
    }
    ofs.close();
    return true;
}

bool Partition_Component::loadLabelling(std::string path)
{
    std::ifstream ifs(path.c_str());
    if (!ifs.is_open())
        return false;

    if (path.substr(path.find_last_of(".") + 1) == "seg")   //[Princeton segmentation benchmark] labelling format
    {
        int n_labels = 0;

        Facet_iterator pFacet	=	m_mesh_ptr->facets_begin();
        for(;pFacet	!= m_mesh_ptr->facets_end();pFacet++)
        {
            ifs >> pFacet->LabelSeg;
            n_labels = std::max(n_labels, pFacet->LabelSeg+1);
        }
        m_mesh_ptr->labelTitles.resize(n_labels);
        for (int i=0; i<n_labels; ++i)
        {
            std::stringstream ss; ss << i;
            m_mesh_ptr->labelTitles[i] = ss.str();
        }
    } else if (path.substr(path.find_last_of(".") + 1) == "txt") //[Kalogerakis 2012] labelling format
    {
        std::vector<int> labels(m_mesh_ptr->size_of_facets(), -1);
        m_mesh_ptr->labelTitles.clear();
        std::set<int> face_ids;

        while(!ifs.eof())
        {
            std::string tmp;
            ifs >> tmp;

            if (is_number(tmp))
            {
                face_ids.insert(atoi(tmp.c_str())-1);
            } else
            {
                if (face_ids.size() > 0)
                {
                    for (std::set<int>::iterator it = face_ids.begin(); it!=face_ids.end(); ++it)
                    {
                        labels[*it] = m_mesh_ptr->labelTitles.size()-1;
                    }
                }
                m_mesh_ptr->labelTitles.push_back(tmp);
                face_ids.clear();
            }
        }

        int k=0;
        Facet_iterator pFacet	=	m_mesh_ptr->facets_begin();
        for(;pFacet	!= m_mesh_ptr->facets_end();pFacet++)
        {
            pFacet->LabelSeg = labels[k];
            ++k;
        }
    }
    return true;
}

bool Partition_Component::saveSubMeshes(std::string prefix_path)
{
    Mesh mesh;  polyhedron2openmesh(m_mesh_ptr, mesh);
    Labelling labelling;

    int k = 0;
    Facet_iterator pFacet	=	m_mesh_ptr->facets_begin();
    for(;pFacet	!= m_mesh_ptr->facets_end();pFacet++)
    {
        labelling[m_mesh_ptr->labelTitles[pFacet->LabelSeg]].insert(k);
        ++k;
    }

    for (Labelling::iterator lit = labelling.begin(); lit!=labelling.end(); ++lit)
    {
        std::stringstream sss;
        sss << prefix_path << "_" << lit->first << ".off";
        OpenMesh::IO::write_mesh(get_submesh(mesh, lit->second), sss.str());
    }
}

void Partition_Component::faceLabelToColorMap()
{
    Facet_iterator pFacet	=	m_mesh_ptr->facets_begin();
    for(;pFacet	!= m_mesh_ptr->facets_end();pFacet++)
    {

        double R=0.0;
        if (m_mesh_ptr->labelTitles.size() > 0)
            R = (double)(pFacet->LabelSeg)/(double)m_mesh_ptr->labelTitles.size()*255.;
        int indiceLut=floor(R);

        pFacet->color(LUT[3*indiceLut],
                LUT[3*indiceLut+1],
                LUT[3*indiceLut+2]);

    }
}

#endif
