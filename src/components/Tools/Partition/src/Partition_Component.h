///////////////////////////////////////////////////////////////////////////
// Author: Flora Tasse
// Year: 2013
// Computer Laboratory-Cambridge University
///////////////////////////////////////////////////////////////////////////
#ifndef Partition_COMPONENT_H
#define Partition_COMPONENT_H


#include <mepp_config.h>
#ifdef BUILD_component_Partition

#include "../../../../mepp/mepp_component.h"
#include "Partition_Polyhedron.h"

class Partition_Component :
  public mepp_component
{
	public:
        enum MeshProperty
        {
            SDFProperty = 0,
            HKSProperty = 1,
            LPEigProperty = 2
        };

        Partition_Component(Viewer* v, PolyhedronPtr p);
        ~Partition_Component() {}

        void segment();
        void faceLabelToColorMap();
        bool saveLabelling(std::string path);
        bool loadLabelling(std::string path);
        bool saveSubMeshes(std::string prefix_path);

        PolyhedronPtr getMeshPtr()
        {
            return m_mesh_ptr;
        }

    private:
        PolyhedronPtr m_mesh_ptr;

        double LUT[3*256];

    public:
        int numInitLabels;
        bool isRecursive, isHierarchical;
        double lambda, alpha;
        MeshProperty meshProperty;
};

#endif

#endif // Partition_COMPONENT_H
