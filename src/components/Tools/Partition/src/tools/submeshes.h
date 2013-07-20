#ifndef OPENMESH_TOOLS_SUBMESHES_H
#define OPENMESH_TOOLS_SUBMESHES_H

#include <set>
#include <queue>

#ifdef __GXX_EXPERIMENTAL_CXX0X__
#include <unordered_map>
#endif

#include <string>
#include <fstream>
#include <OpenMesh/Core/Utils/Property.hh>

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

struct FaceInfo
{
    int fid;
    std::vector<int> neighs;

    FaceInfo(int _fid = -1):fid(_fid){ neighs.reserve(3); }
    void add_neighbour(int other) { neighs.push_back(other); }
};

#ifdef __GXX_EXPERIMENTAL_CXX0X__
typedef std::unordered_map<int,FaceInfo> FaceGraph;
typedef std::unordered_map<std::string, std::set<int> > Labelling;
typedef std::unordered_map<int, bool> GMask;
#else
typedef std::map<int,FaceInfo> FaceGraph;
typedef std::map<std::string, std::set<int> > Labelling;
typedef std::map<int, bool> GMask;
#endif

void get_subgraph(FaceGraph& biggraph, std::set<int>& face_ids, FaceGraph &graph)
{
    graph.clear();
    for (std::set<int>::iterator it = face_ids.begin(); it!=face_ids.end(); ++it)
    {
        FaceInfo &faceinfo = biggraph[*it];
        FaceInfo newfaceinfo(*it);
        for (unsigned int i=0; i<faceinfo.neighs.size(); ++i)
        {
            if (std::find(face_ids.begin(), face_ids.end(), faceinfo.neighs[i]) != face_ids.end())
            {
                newfaceinfo.add_neighbour(faceinfo.neighs[i]);
            }
        }
        graph[*it] = newfaceinfo;
    }
    return;
}

void get_subgraphs(FaceGraph& graph, std::vector<FaceGraph>& graphs)
{
    graphs.clear();

    GMask visited;
    GMask queued;

    FaceGraph::iterator mit  = graph.begin();
    for (mit=graph.begin(); mit!=graph.end(); ++mit)
    {
        visited[mit->first] = false;
        queued[mit->first] = false;
    }

    for (mit=graph.begin(); mit!=graph.end(); ++mit)
    {
        int fid = mit->first;
        if (!visited[fid])
        {
            std::set<int> face_ids;
            std::queue<int> fhs;
            fhs.push(fid);
            queued[fid]  = true;

            while (!fhs.empty())
            {
                int cur_fid = fhs.front();
                fhs.pop();
                if (!visited[cur_fid])
                {
                    visited[cur_fid] = true;
                    face_ids.insert(cur_fid);

                    FaceInfo& faceinfo = graph[cur_fid];
                    for (unsigned int i=0; i<faceinfo.neighs.size(); ++i)
                    {
                        int neigh_fid = faceinfo.neighs[i];
                        if (!queued[neigh_fid] && !visited[neigh_fid])
                        {
                            fhs.push(neigh_fid);
                            queued[neigh_fid] = true;
                        }
                    }
                }
            }

            FaceGraph subgraph;
            get_subgraph(graph, face_ids, subgraph);
            graphs.push_back(subgraph);
        }
    }
}

void graph2faceids(FaceGraph& graph, std::set<int>& face_ids)
{
    face_ids.clear();
    FaceGraph::iterator mit  = graph.begin();
    for (mit=graph.begin(); mit!=graph.end(); ++mit)
        face_ids.insert(mit->first);
}

bool open_mesh_labelling(FaceGraph& graph, std::string label_path, Labelling& labelling)
{
    labelling.clear();
    std::string label_id = "";
    std::set<int> face_ids;
    std::ifstream ifs(label_path.c_str());

    if (!ifs.is_open())
    {
        std::cout << "Could not open labelling file: " << label_path << "\n" << std::flush;
        return false;
    }

    if (label_path.substr(label_path.find_last_of(".") + 1) == "txt")  //[Kalogerakis 2012] labelling format
    {
        while(!ifs.eof())
        {
            std::string tmp;
            ifs >> tmp;
            if (is_number(tmp))
            {
                face_ids.insert(atoi(tmp.c_str())-1);
            } else
            {
                if (face_ids.size() > 0)
                {
                    FaceGraph subgraph;
                    get_subgraph(graph, face_ids, subgraph);
                    std::vector<FaceGraph> subsubgraphs;
                    get_subgraphs(subgraph, subsubgraphs);  //Because a submesh may have disjoint components

                    for (unsigned int k=0; k<subsubgraphs.size(); ++k)
                    {
                        std::set<int> tmp_face_ids;
                        graph2faceids(subsubgraphs[k], tmp_face_ids);
                        std::stringstream sss;
                        if ( subsubgraphs.size() > 1) sss << label_id << k;
                        else sss << label_id;

                        labelling[sss.str()] = tmp_face_ids;
                    }
                }
                label_id = tmp;
                face_ids.clear();
            }
        }
    } else if (label_path.substr(label_path.find_last_of(".") + 1) == "seg")   //[Princeton segmentation benchmark] labelling format
    {
        int n_faces = graph.size();
        for (int i=0; i<n_faces; ++i)
        {
            std::string label_id;
            ifs >> label_id;
            labelling[label_id].insert(i);
        }
    }
    ifs.close();
    return true;
}

void labelling2vector(Labelling& labelling, std::vector<std::string>& face_labels, int n_faces)
{
    face_labels.resize(n_faces, "null");
    for (Labelling::iterator lit=labelling.begin(); lit!=labelling.end(); ++lit)
    {
        std::string label_id = lit->first;
        std::set<int>& face_ids = lit->second;
        for (std::set<int>::iterator it = face_ids.begin(); it != face_ids.end(); ++it)
            face_labels[*it] = label_id;
    }
}

void labelvector2labelling(std::vector< std::set<int> >& label_vector, Labelling &labelling)
{
    labelling.clear();
    for (int i=0; i<label_vector.size(); ++i)
    {
        std::stringstream ss; ss << i;
        labelling[ss.str()] = label_vector[i];
    }
}

void labelling2vector(Labelling& labelling, std::vector<int>& face_labels, int n_faces)
{
    face_labels.resize(n_faces, -1);
    int lbl = 0;
    for (Labelling::iterator lit=labelling.begin(); lit!=labelling.end(); ++lit)
    {
        std::set<int>& face_ids = lit->second;
        for (std::set<int>::iterator it = face_ids.begin(); it != face_ids.end(); ++it)
            face_labels[*it] = lbl;
        ++lbl;
    }
}

bool save_mesh_labelling(FaceGraph& graph, std::string label_path, Labelling& labelling)
{
    std::ofstream ofs(label_path.c_str());

    if (!ofs.is_open())
    {
        std::cout << "Could not open labelling file: " << label_path << "\n" << std::flush;
        return false;
    }

    if (label_path.substr(label_path.find_last_of(".") + 1) == "seg")   //[Princeton segmentation benchmark] labelling format
    {
        int n_faces = graph.size();
        std::vector<std::string> face_labels;
        labelling2vector(labelling, face_labels, n_faces);
        for (int i=0; i<n_faces; ++i) ofs << face_labels[i] << "\n";

    } else if (label_path.substr(label_path.find_last_of(".") + 1) == "txt")  //[Kalogerakis 2012] labelling format
    {
        for (Labelling::iterator lit=labelling.begin(); lit!=labelling.end(); ++lit)
        {
            if (is_number(lit->first))  ofs << "lbl" << lit->first << "\n";
            else  ofs << lit->first << "\n";

            std::set<int>& face_ids = lit->second;
            for (std::set<int>::iterator it = face_ids.begin(); it != face_ids.end(); ++it)
                ofs << *it << " ";
            ofs << std::endl;
        }
    }

    ofs.close();
    return true;
}

template<class MeshT>
void mesh2facegraph(MeshT& mesh, FaceGraph& graph)
{
    graph.clear();
    for (typename MeshT::FaceIter fit = mesh.faces_begin(); fit!=mesh.faces_end(); ++fit)
    {
        int fid = fit.handle().idx();
        FaceInfo faceinfo(fid);
        for (typename MeshT::FaceFaceIter ffit = mesh.ff_iter(fit); ffit; ++ffit)
        {
            faceinfo.add_neighbour(ffit.handle().idx());
        }
        graph[fid] = faceinfo;
    }
}

template<class MeshT>
bool open_mesh_labelling(MeshT& mesh, std::string label_path, Labelling& labelling)
{
    FaceGraph graph;
    mesh2facegraph(mesh, graph);
    return open_mesh_labelling(graph, label_path, labelling);
}

template<class MeshT>
bool save_mesh_labelling(MeshT& mesh, std::string label_path, Labelling& labelling)
{
    FaceGraph graph;
    mesh2facegraph(mesh, graph);
    return save_mesh_labelling(graph, label_path, labelling);
}

template<class MeshT>
MeshT get_submesh(MeshT& mesh, std::set<int> face_ids)
{
    MeshT submesh;
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    std::unordered_map<int, typename MeshT::VertexHandle> vertices_map;
#else
    std::map<int, typename MeshT::VertexHandle> vertices_map;
#endif

    std::set<int> vertex_ids;

    for (std::set<int>::iterator it = face_ids.begin(); it != face_ids.end(); ++it)
    {
        typename MeshT::FaceHandle fh = mesh.face_handle(*it);
        for (typename MeshT::FaceVertexIter fvit = mesh.fv_iter(fh); fvit; ++fvit)
        {
            vertex_ids.insert(fvit.handle().idx());
        }
    }

    for (std::set<int>::iterator it = vertex_ids.begin(); it != vertex_ids.end(); ++it)
    {
        typename MeshT::Point point = mesh.point(mesh.vertex_handle(*it));
        typename MeshT::VertexHandle newvh = submesh.add_vertex(point);
        vertices_map[*it] = newvh;
    }

    for (std::set<int>::iterator it = face_ids.begin(); it != face_ids.end(); ++it)
    {
        typename MeshT::FaceHandle fh = mesh.face_handle(*it);
        std::vector<typename MeshT::VertexHandle> vhs; vhs.reserve(3);
        for (typename MeshT::FaceVertexIter fvit = mesh.fv_iter(fh); fvit; ++fvit)
        {
           vhs.push_back(vertices_map[fvit.handle().idx()]);
        }
        submesh.add_face(vhs);
    }
    return submesh;
}


#endif //OPENMESH_TOOLS_SUBMESHES_H
