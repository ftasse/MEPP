#ifndef SPECTRAL_PROPERTIES_H
#define SPECTRAL_PROPERTIES_H

#include <Eigen/Eigen>
#include <Eigen/Sparse>

void compute_eigen_decomp_arpack(Eigen::SparseMatrix<double> &_A,
                                 Eigen::SparseMatrix<double> &_B,
                                 Eigen::VectorXd &eigvals,
                                 Eigen::MatrixXd &eigvecs,
                                 int nb_eigs);

void compute_eigen_decomp_arpack(Eigen::SparseMatrix<double> &_A,
                                 Eigen::VectorXd &eigvals,
                                 Eigen::MatrixXd &eigvecs,
                                 int nb_eigs);

#endif //SPECTRAL_PROPERTIES_H
