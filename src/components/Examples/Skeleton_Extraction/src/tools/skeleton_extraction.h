#ifndef SKELETON_EXTRACTION_H
#define SKELETON_EXTRACTION_H

#include <string>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

class Skeleton;

namespace skeletonExtraction
{
    struct MyTraits : OpenMesh::DefaultTraits
    {
    typedef OpenMesh::Vec3d Point;
    typedef OpenMesh::Vec3d Normal;
    };
    typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits>  OMesh;

    void contractMesh(OMesh &mesh, OMesh &contracted,
                      double wl0=0.0, double wh0=1.0, double sl=2.0, int maxiter=20);

    void simplifyToSkeleton(OMesh &mesh, OMesh &contracted, Skeleton &skeleton,
                            double wShapeCost = 1.0, double wSamplingCost = 0.1);

    void refineEmbedding(OMesh &mesh, OMesh &contracted, Skeleton &skeleton, bool useCentroids = false);

    void extractSkeleton(OMesh &mesh, OMesh &contracted, Skeleton &skeleton,
                         double wl0=0.0, double wh0=1.0, double sl=2.0, int maxiter=20,
                         double wShapeCost = 1.0, double wSamplingCost = 0.1,
                         bool useCentroids = false, bool save_intermediate = false);
}

bool extractSkeleton(std::string mesh_path, std::string skeleton_path, bool save_intermediate = false);

#endif //SKELETON_EXTRACTION_H
