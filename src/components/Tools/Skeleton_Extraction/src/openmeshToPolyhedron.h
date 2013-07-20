template <class HDS, class OMeshT>
class Build_From_OpenMesh : public CGAL::Modifier_base<HDS> {
public:
    Build_From_OpenMesh(OMeshT &_oMesh):oMesh(_oMesh) {}
    void operator()( HDS& hds) {
        // Postcondition: hds is a valid polyhedral surface.
        CGAL::Polyhedron_incremental_builder_3<HDS> builder( hds, true);
        typedef typename HDS::Vertex   Vertex;
        typedef typename Vertex::Point Point;

        builder.begin_surface(oMesh.n_vertices(), oMesh.n_faces());

        for (unsigned int i = 0; i < oMesh.n_vertices(); i++)
        {
            typename OMeshT::Point pt = oMesh.point(oMesh.vertex_handle(i));
            builder.add_vertex(Point(pt[0], pt[1], pt[2]));
        }

        for (unsigned int i = 0; i < oMesh.n_faces(); i++)
        {
            builder.begin_facet();

            typename OMeshT::FaceVertexIter fvit = oMesh.fv_iter(oMesh.face_handle(i));
            for (; fvit; ++fvit)
            {
                builder.add_vertex_to_facet(fvit.handle().idx());
            }

            builder.end_facet();
        }

        builder.end_surface();
        if (builder.check_unconnected_vertices())   builder.remove_unconnected_vertices();
    }
private:
    OMeshT &oMesh;
};

template <class OMeshT>
void openmesh2polyhedron(OMeshT &oMesh, PolyhedronPtr pMesh)
{
    if (pMesh == 0)
        pMesh = PolyhedronPtr(new Polyhedron());

    Build_From_OpenMesh<HalfedgeDS, OMeshT> builder(oMesh);
    pMesh->delegate(builder);
}

void polyhedron2openmesh(PolyhedronPtr pMesh, skeletonExtraction::OMesh &oMesh)
{
    oMesh.clear();
    pMesh->set_index_vertices();

    Vertex_iterator pVertex	=	pMesh->vertices_begin();
    for(;pVertex != pMesh->vertices_end();pVertex++)
    {
        Enriched_kernel::Point_3 pt = pVertex->point();
        oMesh.add_vertex(skeletonExtraction::OMesh::Point(pt[0], pt[1], pt[2]));
    }

    Facet_iterator pFacet	=	pMesh->facets_begin();
    for(;pFacet	!= pMesh->facets_end();pFacet++)
    {
        std::vector<skeletonExtraction::OMesh::VertexHandle> vhandles;
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
