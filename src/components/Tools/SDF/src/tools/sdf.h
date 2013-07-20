#include <string>
#include <vector>
#include <math.h>

#ifndef USE_CUSTOM_POLYHEDRON
#include "../SDF_Polyhedron.h"
typedef Enriched_kernel Kernel;
#else
#include "polyhedron.h"
#endif

#define DEFAULT_CONE_ANGLE 120.0*M_PI/180.0
#define DEFAULT_NB_CONERAYS 30
#define DEFAULT_NB_SMOOTHING_ITER 5

std::vector<double> compute_face_sdf(Polyhedron &P,
                                     std::vector<double> &sdf_values,
                                     double cone_ang=DEFAULT_CONE_ANGLE,
                                     int nb_cone_rays=DEFAULT_NB_CONERAYS,
                                     int nb_smoothing_iter=DEFAULT_NB_SMOOTHING_ITER);

std::vector<double> compute_vertex_sdf(Polyhedron &P,
                                       std::vector<double> &sdf_values,
                                       double cone_ang=DEFAULT_CONE_ANGLE,
                                       int nb_cone_rays=DEFAULT_NB_CONERAYS,
                                       int nb_smoothing_iter=DEFAULT_NB_SMOOTHING_ITER);

std::vector<double> compute_face_sdf(std::string mesh_path,
                                     double cone_ang=DEFAULT_CONE_ANGLE,
                                     int nb_cone_rays=DEFAULT_NB_CONERAYS,
                                     int nb_smoothing_iter=DEFAULT_NB_SMOOTHING_ITER);

std::vector<double> compute_vertex_sdf(std::string mesh_path,
                                       double cone_ang=DEFAULT_CONE_ANGLE,
                                       int nb_cone_rays=DEFAULT_NB_CONERAYS,
                                       int nb_smoothing_iter=DEFAULT_NB_SMOOTHING_ITER);
