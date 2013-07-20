#ifndef SIHKS_H
#define SIHKS_H

#include <Eigen/Eigen>

template<typename Mesh>
Eigen::MatrixXd compute_spectra(Mesh &mesh, Eigen::VectorXd &eigvals,
                            Eigen::MatrixXd &eigvecs, int nev = 300);

template<typename Matrix>
Matrix compute_hks(Eigen::VectorXd &eigvals,
                            Matrix &eigvecs, int nstep);

template<typename Mesh>
Eigen::MatrixXd compute_hks(Mesh &mesh, int nev, int nstep = 300, bool scale = true);

#ifndef SIHKS_CPP
#include "hks.cpp"
#endif

#endif //SIKHS_H


