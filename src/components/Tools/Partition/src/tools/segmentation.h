#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <string>
#include <vector>
#include <set>

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <opencv2/opencv.hpp>

typedef OpenMesh::PolyMesh_ArrayKernelT<> Mesh;


struct GMMFit
{
    cv::Mat* samples;
    int num_labels;
    bool use_faces;
    std::vector< int > labels;
    std::vector< std::vector<double> > probs;

    GMMFit(std::vector<double>& weights, int n_labels, bool _use_faces = true);
    GMMFit(std::vector< std::vector<double> >& weights, int n_labels, bool _use_faces = true);
    ~GMMFit();

    void train(std::vector<int> mask_ids);
};

void segmentation(std::string mesh_path, GMMFit& gmmfit,
                  std::vector< std::set<int> >& labelling,
                  bool recursive = false, bool hierarchical = false);

void segmentation(Mesh &mesh, GMMFit& gmmfit,
                  std::vector< std::set<int> >& labelling,
                  bool recursive = false, bool hierarchical = false,
                  double lambda = 25, double alpha = 0.5);

#endif //SEGMENTATION_H
