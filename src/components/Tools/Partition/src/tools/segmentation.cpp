#include <queue>

#include <opencv2/ml/ml.hpp>
#include <OpenMesh/Core/IO/MeshIO.hh>

#include "gco/GCoptimization.h"
#include "segmentation.h"

void calc_edge_min_weights(Mesh &mesh, Mesh::HalfedgeHandle heh, double &ang, double &len)
{
    double theta = mesh.calc_dihedral_angle(heh);
    /*double weight =std::abs(theta/M_PI);
    if ( theta > 0.0 ) weight*=5;*/

    double convexFac = 1;
    if (theta > 0.0)
        convexFac = 5;
    double weight = pow(1 - (1-cos(theta))*convexFac, 2);

    ang = weight;
    len = mesh.calc_edge_length(heh);
}

bool mincut(Mesh &mesh, std::vector<int>& region, GMMFit& gmmfit,
            std::vector<int>& labels, double lambda, double alpha)
{
    if (region.size() <= (unsigned int)gmmfit.num_labels*gmmfit.num_labels)
        return false;

    int num_values = region.size();
    int num_labels = gmmfit.num_labels;
    gmmfit.train(region);
    labels = gmmfit.labels;

    int *data = new int[num_values*num_labels];
    for ( int i = 0; i < num_values; i++ )
        for (int l = 0; l < num_labels; l++ )
        {
            data[i*num_labels + l] = -log(gmmfit.probs[i][l] + 1e-8);
            //std::cout <<gmmfit.face_probs[i][l] << " " << -log(gmmfit.face_probs[i][l] + 1e-8)<< "\n" << std::flush;
        }
    int *smooth = new int[num_labels*num_labels];
    for ( int l1 = 0; l1 < num_labels; l1++ )
        for (int l2 = 0; l2 < num_labels; l2++ )
            smooth[l1+l2*num_labels] = (l1==l2)? 0:lambda;

    try{
        GCoptimizationGeneralGraph *gc = new GCoptimizationGeneralGraph(num_values,num_labels);
        gc->setDataCost(data);
        gc->setSmoothCost(smooth);

        std::map<int, int> id_map;
        for (unsigned int i=0; i<region.size(); ++i)
            id_map[region[i]] = i;

        std::vector< std::pair<int, int> > edges;
        std::vector< std::pair<double, double> > edgeinfo;  //Angle & weights
        double avg_ang = 0.0;
        double avg_len = 0.0;

        for (unsigned int i=0; i<region.size(); ++i)
        {
            if (gmmfit.use_faces)
            {
                Mesh::FaceHandle fh = mesh.face_handle(region[i]);
                for (Mesh::FaceHalfedgeIter fhit = mesh.fh_iter(fh); fhit; ++fhit)
                {
                    int other_fid = mesh.face_handle(mesh.opposite_halfedge_handle(fhit.handle())).idx();
                    if (other_fid < 0 || id_map.find(other_fid) == id_map.end())
                        continue;

                    double ang, len;
                    calc_edge_min_weights(mesh, fhit.handle(), ang, len);
                    avg_ang += ang; avg_len += len;
                    edges.push_back(std::pair<int, int>(i, id_map[other_fid]));
                    edgeinfo.push_back(std::pair<double, double>(ang, len));
                }
            } else // use vertices
            {
                Mesh::VertexHandle vh = mesh.vertex_handle(region[i]);
                for (Mesh::VertexOHalfedgeIter vohit = mesh.voh_iter(vh); vohit; ++vohit)
                {
                    int other_vid = mesh.to_vertex_handle(vohit.handle()).idx();
                    if (other_vid < 0 || id_map.find(other_vid) == id_map.end())
                        continue;

                    double ang, len;
                    calc_edge_min_weights(mesh, vohit.handle(), ang, len);
                    avg_ang += ang; avg_len += len;
                    edges.push_back(std::pair<int, int>(i, id_map[other_vid]));
                    edgeinfo.push_back(std::pair<double, double>(ang, len));
                }
            }
        }

        if (edges.size() > 0)
        {
            //avg_ang /= edges.size();
            //avg_len /= edges.size();
        }

        for (unsigned int i=0; i<edges.size(); ++i)
        {
            double weight = alpha*(edgeinfo[i].first/avg_ang) + (1-alpha)*(edgeinfo[i].second/avg_len);

            if (weight < 0.0 || weight > 1.0)
                std::cout << weight << "\n" << std::flush;

            gc->setNeighbors(edges[i].first, edges[i].second,  -log(std::max(weight, 1e-8)));
        }

        printf("Before optimization energy is %lld",gc->compute_energy()); std::cout << std::flush;
        gc->expansion();// run expansion for 2 iterations. For swap use gc->swap(num_iterations);
        printf("\nAfter optimization energy is %lld\n\n",gc->compute_energy()); std::cout << std::flush;

        for ( int  i = 0; i < num_values; i++ ) labels[i] = gc->whatLabel(i);

        delete gc;
    }
    catch (GCException e){
        e.Report();
    }

    delete [] smooth;
    delete [] data;

    bool segmented = false;
    int first_label = labels[0];
    for (unsigned int i=0; i<labels.size(); ++i)
        if (labels[i] != first_label)
        {
            segmented = true;   break;
        }
    return segmented;
}

void segmentation(Mesh &mesh, GMMFit& gmmfit,
                  std::vector< std::set<int> >& labelling,
                  bool recursive, bool hierarchical,
                  double lambda, double alpha)
{
    if (!mesh.has_vertex_normals())
    {
        mesh.request_face_normals();
        mesh.request_vertex_normals();
        mesh.update_normals();
    }

    int n_labels = gmmfit.num_labels;
    int n_values = (gmmfit.use_faces)?mesh.n_faces():mesh.n_vertices();

    labelling.clear();
    labelling.reserve(n_labels);
    std::queue<int> segmentids;

    std::vector<int> removed;

    do
    {
        int sid = -1;
        std::vector<int> segment;

        if (segmentids.empty())
        {
            segment.reserve(n_values);
            for (int i=0; i< n_values; ++i)
                segment.push_back(i);
        } else
        {
            sid = segmentids.front();
            segment.insert(segment.end(), labelling[sid].begin(), labelling[sid].end());
            segmentids.pop();
        }

        std::vector<int> labels;
        if (mincut(mesh, segment, gmmfit, labels, lambda, alpha))
        {
            if (sid >=0)    removed.push_back(sid);
            std::map<int, std::set<int> > result;
            for (unsigned int j=0; j<labels.size(); ++j)
                result[labels[j]].insert(segment[j]);

            for (std::map<int, std::set<int> >::iterator mit = result.begin(); mit != result.end(); ++mit)
            {
                segmentids.push(labelling.size());
                labelling.push_back(mit->second);
            }
        }

    }   while (recursive && !segmentids.empty());


    if (!hierarchical)
        for (int i = (int)removed.size()-1; i>=0; --i)
            labelling.erase(labelling.begin() + removed[i]);
}

void segmentation(std::string mesh_path, GMMFit& gmmfit,
                  std::vector< std::set<int> >& labelling,
                  bool recursive, bool hierarchical)
{
    Mesh mesh;
    if (!OpenMesh::IO::read_mesh(mesh, mesh_path))  return;
    segmentation(mesh, gmmfit, labelling, recursive, hierarchical);
}

GMMFit::GMMFit(std::vector<double>& weights, int n_labels, bool _use_faces)
{
    use_faces = _use_faces;
    num_labels = n_labels;
    samples = new cv::Mat(weights.size(), 1, CV_32FC1);
    for (unsigned int i=0; i<weights.size(); ++i)
    {
        samples->at<float>(i, 0) = weights[i];
    }
}

GMMFit::GMMFit(std::vector< std::vector<double> >& weights, int n_labels, bool _use_faces)
{
    use_faces = _use_faces;
    num_labels = n_labels;
    samples = new cv::Mat(weights.size(), weights[0].size(), CV_32FC1);
    for (unsigned int i=0; i<weights.size(); ++i)
    {
        for (unsigned int j=0; j<weights[i].size(); ++j)
            samples->at<float>(i, j) = weights[i][j];
    }
}

GMMFit::~GMMFit()
{
    delete samples;
}

void GMMFit::train(std::vector<int> mask_ids)
{
    std::cout << "Mask size: " << mask_ids.size() << "\n" << std::flush;
    cv::TermCriteria term_crit(cv::TermCriteria::COUNT+cv::TermCriteria::EPS, 30, 1e-6);

    cv::Mat cvlabels = cv::Mat::zeros(mask_ids.size(), 1, CV_32SC1);
    cv::Mat cvprobs = cv::Mat::zeros(mask_ids.size(), num_labels, CV_64FC1);
    cv::Mat cur_samples = *samples;

    if (mask_ids.size() != samples->rows)
    {
        cur_samples = cv::Mat(mask_ids.size(), samples->cols, samples->type());
        for (int i=0; i<cur_samples.rows; ++i)
            for (int j=0; j<cur_samples.cols; ++j)
                cur_samples.at<float>(i, j) = samples->at<float>(mask_ids[i], j);
    }

#if CV_MAJOR_VERSION>=2 && CV_MINOR_VERSION>=4

    cv::EM em_model(num_labels, cv::EM::COV_MAT_DIAGONAL, term_crit);
    em_model.train(cur_samples, cv::noArray(), cvlabels, cvprobs);

#else

    CvEM em_model;
    CvEMParams em_params;
    em_params.term_crit = term_crit;
    em_params.nclusters = num_labels;
    em_params.cov_mat_type = CvEM::COV_MAT_DIAGONAL;
    em_model.train(cur_samples, cv::Mat(), em_params, &cvlabels);
    cvprobs = em_model.getProbs();

#endif

    labels.resize(mask_ids.size());
    probs.resize(mask_ids.size());
    for (unsigned int i=0; i<labels.size(); ++i)
    {
        labels[i] = cvlabels.at<int>(i, 0);
        probs[i].resize(num_labels);
        for (int k=0; k<num_labels; ++k)
        {
            probs[i][k] = cvprobs.at<double>(i, k);
            //std::cout <<face_probs[i][k] << " ";
        }
        //std::cout << "\n";
    }
}
