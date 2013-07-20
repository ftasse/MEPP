///////////////////////////////////////////////////////////////////////////
// Author: Flora Tasse
// Year: 2013
// Computer Laboratory-Cambridge University
///////////////////////////////////////////////////////////////////////////
#include <mepp_config.h>
#ifdef BUILD_component_SDF

#ifdef _MSC_VER
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#endif

#include "SDF_Component.h"
#include "tools/sdf.h"

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

SDF_Component::SDF_Component(Viewer* v, PolyhedronPtr p):mepp_component(v, p)
{
    initLUTIndex(LUT);
    m_mesh_ptr = v->getScenePtr()->get_polyhedron();

    coneAngInDegs = 120;
    numConeRays = 30;
    numSmoothingIters = 3;
    sdfPerFacet = true;

    // MEPP 2
    componentName = "SDF_Component";
    init = 1;
}

void SDF_Component::computeSDF()
{
    std::vector<double> sdf_values;

    if (!sdfPerFacet)
    {
        compute_vertex_sdf(*m_mesh_ptr, sdf_values, coneAngInDegs*M_PI/180.0, numConeRays,
                         numSmoothingIters);

        int k = 0;
        for (Vertex_iterator pVertex = m_mesh_ptr->vertices_begin();
             pVertex != m_mesh_ptr->vertices_end();
             pVertex++)
        {
            pVertex->sdfVal = sdf_values[k];
            ++k;
        }
    } else
    {
        compute_face_sdf(*m_mesh_ptr, sdf_values, coneAngInDegs*M_PI/180.0, numConeRays,
                         numSmoothingIters);

        int k = 0;
        for (Facet_iterator pFacet = m_mesh_ptr->facets_begin();
             pFacet != m_mesh_ptr->facets_end();
             pFacet++)
        {
            pFacet->sdfVal = sdf_values[k];
            ++k;
        }
    }
}

bool SDF_Component::loadSDF(string path, bool perFacet)
{
    std::ifstream ifs(path.c_str());
    if (!ifs.is_open())  return false;

    int numvals; ifs >> numvals;
    if (numvals == m_mesh_ptr->size_of_facets())
        perFacet = true;
    else if (numvals == m_mesh_ptr->size_of_vertices())
        perFacet = false;
    else
        return false;

    if (!perFacet)
    {
        for (Vertex_iterator pVertex = m_mesh_ptr->vertices_begin();
             pVertex != m_mesh_ptr->vertices_end();
             pVertex++)
        {
            ifs >> pVertex->sdfVal;
        }
    } else
    {
        for (Facet_iterator pFacet = m_mesh_ptr->facets_begin();
             pFacet != m_mesh_ptr->facets_end();
             pFacet++)
        {
            ifs >> pFacet->sdfVal;
        }
    }
    ifs.close();
    return true;
}

bool SDF_Component::saveSDF(string path, bool perFacet)
{
    std::ofstream ofs(path.c_str());
    if (!ofs.is_open())  return false;

    int numvals = perFacet?m_mesh_ptr->size_of_facets():m_mesh_ptr->size_of_vertices();
    ofs << numvals;

    if (!perFacet)
    {
        for (Vertex_iterator pVertex = m_mesh_ptr->vertices_begin();
             pVertex != m_mesh_ptr->vertices_end();
             pVertex++)
        {
            ofs << " " << pVertex->sdfVal;
        }
    } else
    {
        for (Facet_iterator pFacet = m_mesh_ptr->facets_begin();
             pFacet != m_mesh_ptr->facets_end();
             pFacet++)
        {
            ofs << " " << pFacet->sdfVal;
        }
    }
    ofs << "\n";
    ofs.close();
    return true;
}

void SDF_Component::sdfToColorMap(bool perFacet)
{
    double R;
    int indiceLut;
    Vertex_iterator pVertex = NULL;
    Facet_iterator pFacet = NULL;

    double minSDF = DBL_MAX;
    double maxSDF = DBL_MIN;

    if (!perFacet)
    {
        for (pVertex = m_mesh_ptr->vertices_begin();
             pVertex != m_mesh_ptr->vertices_end();
             pVertex++)
        {
            minSDF = std::min(pVertex->sdfVal, minSDF);
            maxSDF = std::max(pVertex->sdfVal, maxSDF);
        }

        for (pVertex = m_mesh_ptr->vertices_begin();
             pVertex != m_mesh_ptr->vertices_end();
             pVertex++)
        {

            double R=(pVertex->sdfVal-minSDF)/(maxSDF-minSDF)*255;
            indiceLut=floor(std::min(R, 255.0));
            pVertex->color(LUT[3*indiceLut],
                    LUT[3*indiceLut+1],
                    LUT[3*indiceLut+2],
                    0.6f);
        }
    }  else
    {
        for (pFacet = m_mesh_ptr->facets_begin();
             pFacet != m_mesh_ptr->facets_end();
             pFacet++)
        {
            minSDF = std::min(pFacet->sdfVal, minSDF);
            maxSDF = std::max(pFacet->sdfVal, maxSDF);
        }

        for (pFacet = m_mesh_ptr->facets_begin();
             pFacet != m_mesh_ptr->facets_end();
             pFacet++)
        {

            double R=(pFacet->sdfVal-minSDF)/(maxSDF-minSDF)*255;
            indiceLut=floor(std::min(R, 255.0));

            pFacet->color(LUT[3*indiceLut],
                    LUT[3*indiceLut+1],
                    LUT[3*indiceLut+2]);
        }
    }

}

#endif
