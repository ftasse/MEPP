#include <OpenMesh/Core/IO/MeshIO.hh>
#include "skeleton_extraction.h"
#include "skeleton.h"

int main(int argc, char**argv)
{
    if (argc < 3)
    {
        std::cout << "Usage:\n" << argv[1] << "  mesh_in_file skeleton_out_file\n\n" << std::flush;
        return 1;
    }

    std::string mesh_path = argv[1];
    std::string skel_path = argv[2];

    skeletonExtraction::OMesh mesh, contracted;
    Skeleton skeleton;
    if (!OpenMesh::IO::read_mesh(mesh, mesh_path.c_str()))
    {
        std::cout << "Could not open mesh at path: " << mesh_path << std::endl << std::flush;
        return 1;
    }
    std::cout << "nb of vertices: " << mesh.n_vertices() << std::endl << std::flush;
    std::cout << "nb of faces: " << mesh.n_faces() << std::endl << std::flush;

    skeletonExtraction::extractSkeleton(mesh, contracted, skeleton);
    skeleton.save(skel_path);
    return 0;
}
