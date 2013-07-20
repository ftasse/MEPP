///////////////////////////////////////////////////////////////////////////
// Author: Flora Tasse
// Year: 2013
// Computer Laboratory-Cambridge University
///////////////////////////////////////////////////////////////////////////
#ifndef SDF_COMPONENT_H
#define SDF_COMPONENT_H


#include <mepp_config.h>
#ifdef BUILD_component_SDF

#include "../../../../mepp/mepp_component.h"
#include "SDF_Polyhedron.h"

class SDF_Component :
  public mepp_component
{
	public:
        SDF_Component(Viewer* v, PolyhedronPtr p);
        ~SDF_Component() {}

        void computeSDF();
        void sdfToColorMap(bool perFacet = true);

        bool loadSDF(std::string path, bool perFacet = true);
        bool saveSDF(std::string path, bool perFacet = true);

        PolyhedronPtr getMeshPtr()
        {
            return m_mesh_ptr;
        }

    private:
        PolyhedronPtr m_mesh_ptr;

        double LUT[3*256];

    public:
        int coneAngInDegs, numConeRays, numSmoothingIters;
        bool sdfPerFacet;
};

#endif

#endif // SDF_COMPONENT_H
