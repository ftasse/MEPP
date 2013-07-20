#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <Eigen/Sparse>

//#include <Eigen/UmfPackSupport>
#include "Eigen3ArpackSupport/ArpackSupport"

#include "eigendecomposition.h"
#include "generate_lpmatrix.h"

typedef OpenMesh::PolyMesh_ArrayKernelT<> PMesh;
typedef Eigen::SparseMatrix<double> Matrix;
typedef Eigen::SimplicialLDLT<Matrix> LinearSolver;
//typdef Eigen::UmfPackLU<Matrix> LinearSolver;

void compute_eigen_decomp_arpack(Eigen::SparseMatrix<double> &_A,
                                 Eigen::VectorXd &eigvals,
                                 Eigen::MatrixXd &eigvecs,
                                 int nb_eigs)
{
    Eigen::ArpackGeneralizedSelfAdjointEigenSolver<Matrix, LinearSolver > eig_solver;
    eig_solver.compute(_A, nb_eigs, "SM", Eigen::ComputeEigenvectors);
    eigvals = eig_solver.eigenvalues();
    eigvecs = eig_solver.eigenvectors();

    eigvals.reverseInPlace();
    for (int i=0; i<eigvecs.rows(); ++i)
        eigvecs.row(i).reverseInPlace();
}

void compute_eigen_decomp_arpack(Eigen::SparseMatrix<double> &_A,
                                 Eigen::SparseMatrix<double> &_B,
                                 Eigen::VectorXd &eigvals,
                                 Eigen::MatrixXd &eigvecs,
                                 int nb_eigs)
{
    Eigen::ArpackGeneralizedSelfAdjointEigenSolver<Matrix, LinearSolver > eig_solver;
    eig_solver.compute(_A, _B, nb_eigs, "SM", Eigen::ComputeEigenvectors);
    eigvals = eig_solver.eigenvalues();
    eigvecs = eig_solver.eigenvectors();

    eigvals.reverseInPlace();
    for (int i=0; i<eigvecs.rows(); ++i)
        eigvecs.row(i).reverseInPlace();
}
