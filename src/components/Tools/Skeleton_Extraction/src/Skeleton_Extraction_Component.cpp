///////////////////////////////////////////////////////////////////////////
// Author: Flora Tasse
// Year: 2013
// Computer Laboratory-Cambridge University
///////////////////////////////////////////////////////////////////////////
#include <mepp_config.h>
#ifdef BUILD_component_Skeleton_Extraction

#ifdef _MSC_VER
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#endif

#include "Skeleton_Extraction_Component.h"
#include "tools/skeleton_extraction.h"
#include "openmeshToPolyhedron.h"

#include <time.h>

// subdivision
#include "../../../../mepp/Tools/Tools_sqrt3.h"
#include "../../../../mepp/viewer.hxx"

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

Skeleton_Extraction_Component::Skeleton_Extraction_Component(Viewer* v, PolyhedronPtr p):mepp_component(v, p)
{
    initLUTIndex(LUT);
    m_mesh_ptr = v->getScenePtr()->get_polyhedron();
    m_contractedmesh_ptr = PolyhedronPtr(new Polyhedron());
    m_skeleton_ptr = SkeletonPtr(new Skeleton());

    Facet_iterator pFacet	=	m_mesh_ptr->facets_begin();
    double totArea = 0.0;
    for(;pFacet	!= m_mesh_ptr->facets_end();pFacet++)
    {
        pFacet->color(pFacet->color(0), pFacet->color(1), pFacet->color(2), 0.2f);
        totArea += Enriched_kernel::Compute_area_3()(pFacet->halfedge()->vertex()->point(),
                                                     pFacet->halfedge()->next()->vertex()->point(),
                                                     pFacet->halfedge()->opposite()->vertex()->point() );
    }

    //Contraction
    wl0 = sqrt(totArea/m_mesh_ptr->size_of_facets());
    wh0 = 1.0;
    sl = 2.0;
    maxiter = 20;

    //Simplification
    wShapeCost = 1.0;
    wSamplingCost = 0.1;

    //Embedding refinement
    useCentroid  = false;


    // MEPP 2
    componentName = "Skeleton_Extraction_Component";
    init = 1;
}

void Skeleton_Extraction_Component::contract()
{
    skeletonExtraction::OMesh mesh, contracted;
    polyhedron2openmesh(m_mesh_ptr, mesh);

    skeletonExtraction::contractMesh(mesh, contracted, wl0, wh0, sl, maxiter);
    openmesh2polyhedron(contracted, m_contractedmesh_ptr);
}

void Skeleton_Extraction_Component::simplify()
{
    if (m_contractedmesh_ptr->size_of_vertices() == 0)
        return;

    skeletonExtraction::OMesh mesh, contracted;
    polyhedron2openmesh(m_mesh_ptr, mesh);
    polyhedron2openmesh(m_contractedmesh_ptr, contracted);

    skeletonExtraction::simplifyToSkeleton(mesh, contracted, *m_skeleton_ptr,
                                           wShapeCost, wSamplingCost);
}

void Skeleton_Extraction_Component::refineEmbedding()
{
    if (m_contractedmesh_ptr->size_of_vertices() == 0 ||
            m_skeleton_ptr->verts.size() == 0)
        return;

    skeletonExtraction::OMesh mesh, contracted;
    polyhedron2openmesh(m_mesh_ptr, mesh);
    polyhedron2openmesh(m_contractedmesh_ptr, contracted);

    skeletonExtraction::refineEmbedding(mesh, contracted, *m_skeleton_ptr, useCentroid);
}

void Skeleton_Extraction_Component::extractSkeleton()
{
    contract();
    simplify();
    refineEmbedding();
}

#endif

void Skeleton_Extraction_Component::vertexThicknessToColorMap()
{
    double R;
    int indiceLut;
    Vertex_iterator pVertex = NULL;

    double minThickness = DBL_MAX;
    double maxThickness = DBL_MIN;
    m_mesh_ptr->set_index_vertices();

    std::vector<int> skel_node(m_mesh_ptr->size_of_vertices(), -1);
    for (unsigned int i=0; i<m_skeleton_ptr->verts.size(); ++i)
    {
        for (unsigned int j=0; j< m_skeleton_ptr->regions[i].size(); ++j)
            skel_node[m_skeleton_ptr->regions[i][j]] = i;
    }

    for (pVertex = m_mesh_ptr->vertices_begin();
         pVertex != m_mesh_ptr->vertices_end();
         pVertex++)
    {
        int node = skel_node[pVertex->tag()];
        Skeleton::Vec3 spt = m_skeleton_ptr->verts[node];
        Enriched_kernel::Point_3 pt(spt[0], spt[1], spt[2]);
        pVertex->thickness() = sqrt((pt-pVertex->point())*(pt-pVertex->point()));
        minThickness = std::min(pVertex->thickness(), minThickness);
        maxThickness = std::max(pVertex->thickness(), maxThickness);
    }

    for (pVertex = m_mesh_ptr->vertices_begin();
         pVertex != m_mesh_ptr->vertices_end();
         pVertex++)
    {
        R = (pVertex->thickness()-minThickness)/(maxThickness-minThickness)*255;
        indiceLut = floor(std::min(R, 255.0));
        pVertex->color(LUT[3*indiceLut],
                LUT[3*indiceLut+1],
                LUT[3*indiceLut+2],
                0.6f);
    }
}
