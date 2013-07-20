#ifndef MESHSIMPLIFICATIONH
#define MESHSIMPLIFICATIONH

class MeshToSkeleton;

template <class M>
MeshToSkeleton simplify(M &mesh, double wa = 1.0, double wb = 0.1, bool use_plane_quadrics = false);

#ifndef MESHSIMPLIFICATIONHXX
#include "mesh_simplification.hxx"
#endif

#endif // MESHSIMPLIFICATIONH
