#include <fstream>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include "OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh"
#include "hks.h"

int main(int argc, char**argv)
{
    typedef OpenMesh::TriMesh_ArrayKernelT<> Mesh;
    if (argc < 2)
    {
        std::cout << "Usage:\n" << argv[1] << "  mesh_in_file hks_out_file\n\n" << std::flush;
        return 1;
    }

    std::string mesh_path = argv[1];
    std::string hks_path = argc>2?argv[2]:"";

    Mesh mesh;
    if (!OpenMesh::IO::read_mesh(mesh, mesh_path.c_str()))
    {
        std::cout << "Could not open mesh at path: " << mesh_path << std::endl << std::flush;
        return 1;
    }
    std::cout << "nb of vertices: " << mesh.n_vertices() << std::endl << std::flush;
    std::cout << "nb of faces: " << mesh.n_faces() << std::endl << std::flush;

    Eigen::MatrixXd hks_mat = compute_hks(mesh, 300);
    
    if (hks_path.size() > 0)
    {
        std::ofstream ofs(hks_path.c_str());
        ofs << hks_mat;
        ofs.close();
    } else
        std::cout << hks_mat;
    return 0;
}
