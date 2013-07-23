#ifndef SIHKS_H
#define SIHKS_H

#include <Eigen/Eigen>

#include <iostream>

#include "eigendecomposition.h"
#include "generate_lpmatrix.h"


template<typename Mesh>
Eigen::SparseMatrix<double> compute_spectra(Mesh &mesh, Eigen::VectorXd &eigvals,
                            Eigen::MatrixXd &eigvecs, int nev)
{
    Eigen::SparseMatrix<double> W, A;

    compute_laplacian_matrix_Pinkall_Polthier(mesh, W, A);
    compute_eigen_decomp_arpack(W, A, eigvals, eigvecs, nev);
    std::cout << eigvals.size() << "th eigenvalue: " << eigvals(eigvals.size()-1) << "\n" << std::flush;

    return A;
}

template<typename Matrix>
Matrix compute_hks(Eigen::VectorXd &eigvals,
                            Matrix &eigvecs, int nstep = 100)
{
    double maxeigv = eigvals(eigvals.size()-1);
    double mineigv = eigvals(1);
    double tmin = std::abs(4*log(10) / maxeigv);
    double tmax = std::abs(4*log(10) / mineigv);

    double ltmin = log(tmin), ltmax = log(tmax);
    double stepsize = (ltmax - ltmin) / nstep;
    Eigen::RowVectorXd ts(nstep+1);
    for (unsigned int i=0; i<ts.cols(); ++i)
        ts(i) = exp( ltmin + i*stepsize);

    //|psi[k]|^2 for k>0
    Matrix T1 = eigvecs.block(0, 1, eigvecs.rows(), eigvecs.cols()-1).array().pow(2);
    //exp(-|lamda[k]|*t) for k>0
    Matrix T2 = (- eigvals.block(1, 0, eigvals.rows()-1, eigvals.cols()).array().abs().matrix() * ts).array().exp();

    Matrix hks = (T1*T2).array().abs();
    return hks;
}

template<typename Mesh>
Eigen::MatrixXd compute_hks(Mesh &mesh, int nev, int nstep = 100, bool scale = true)
{
    Eigen::VectorXd eigvals;
    Eigen::MatrixXd eigvecs;

    Eigen::SparseMatrix<double> A = compute_spectra(mesh, eigvals, eigvecs, nev);
    Eigen::MatrixXd hks = compute_hks(eigvals, eigvecs, nstep);

    if (scale)
    {
        Eigen::VectorXd scale = ((A*hks).colwise().sum()).array().pow(-1); // 1.0/(A*hks)
        hks = hks * scale.asDiagonal();
    }

    //std::cout << "HKS: \n" << hks.block(0, 0, 5, 5) << "\n" << std::flush;
    return hks;
}


#endif //SIKHS_H


