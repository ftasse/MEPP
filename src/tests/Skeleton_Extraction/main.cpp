#include <OpenMesh/Core/IO/MeshIO.hh>
#include "skeleton_extraction.h"

int main(int argc, char**argv)
{
    if (argc < 2)
    {
        std::cout << "Usage:\n" << argv[0] << "  mesh_in_file skeleton_out_file\n\n" << std::flush;
        return 1;
    }

    std::string mesh_path = argv[0];
    std::string skel_path = argv[1];

    skeletonExtraction::OMesh mesh, contracted;
    Skeleton skeleton;
    if (!OpenMesh::IO::read_mesh(mesh, mesh_path.c_str()))
    {
        std::cout << "Could not open mesh at path: " << tmp_path << std::endl << std::flush;
        return 1;
    }
    std::cout << "nb of vertices: " << mesh.n_vertices() << std::endl << std::flush;
    std::cout << "nb of faces: " << mesh.n_faces() << std::endl << std::flush;

    skeletonExtraction::extractSkeleton(mesh, contracted, skeleton);
    skeleton.save(skel_path);
    return 0;
}
