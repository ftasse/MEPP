///////////////////////////////////////////////////////////////////////////
// Author: Flora Tasse
// Year: 2013
// Computer Laboratory-Cambridge University
///////////////////////////////////////////////////////////////////////////
#ifndef Skeleton_Extraction_COMPONENT_H
#define Skeleton_Extraction_COMPONENT_H


#include <mepp_config.h>
#ifdef BUILD_component_Skeleton_Extraction

#include "../../../../mepp/mepp_component.h"
#include "Skeleton_Extraction_Polyhedron.h"
#include "tools/skeleton.h"

typedef boost::shared_ptr<Skeleton> SkeletonPtr;

class Skeleton_Extraction_Component :
  public mepp_component
{
	public:
        Skeleton_Extraction_Component(Viewer* v, PolyhedronPtr p);
        ~Skeleton_Extraction_Component() {}

        void contract();
        void simplify();
        void refineEmbedding();
        void extractSkeleton();
        void vertexThicknessToColorMap();

        SkeletonPtr getSkeletonPtr()
        {
            return m_skeleton_ptr;
        }

        PolyhedronPtr getContractedMeshPtr()
        {
            return m_contractedmesh_ptr;
        }

        PolyhedronPtr getMeshPtr()
        {
            return m_mesh_ptr;
        }

        bool loadSkeleton(std::string path)
        {
            return m_skeleton_ptr->open(path);
        }

        bool saveSkeleton(std::string path)
        {
            return m_skeleton_ptr->save(path);
        }

    private:
        SkeletonPtr m_skeleton_ptr;
        PolyhedronPtr m_contractedmesh_ptr;
        PolyhedronPtr m_mesh_ptr;

        double LUT_CourbureClust[3*256];

    public:
        double wl0, wh0, sl, wShapeCost, wSamplingCost;
        int maxiter;
        bool useCentroid;
};

#endif

#endif // Skeleton_Extraction_COMPONENT_H
