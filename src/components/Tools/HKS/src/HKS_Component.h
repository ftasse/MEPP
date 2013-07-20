///////////////////////////////////////////////////////////////////////////
// Author: Flora Tasse
// Year: 2013
// Computer Laboratory-Cambridge University
///////////////////////////////////////////////////////////////////////////
#ifndef HKS_COMPONENT_H
#define HKS_COMPONENT_H


#include <mepp_config.h>
#ifdef BUILD_component_HKS

#include <Eigen/Sparse>
#include "../../../../mepp/mepp_component.h"
#include "HKS_Polyhedron.h"

class HKS_Component :
  public mepp_component
{
	public:
        HKS_Component(Viewer* v, PolyhedronPtr p);
        ~HKS_Component() {}

        void computeSpectra();
        void computeHKS();
        bool saveHKS(std::string path);
        bool loadHKS(std::string path);
        void hksToColorMap();
        void spectraToColorMap();

        PolyhedronPtr getMeshPtr()
        {
            return m_mesh_ptr;
        }

    private:
        PolyhedronPtr m_mesh_ptr;

        double LUT[3*256];

    public:
        int nev, nsteps;
        bool useGeodesic;
        int eig_id, hks_tid;
};

#endif

#endif // HKS_COMPONENT_H
