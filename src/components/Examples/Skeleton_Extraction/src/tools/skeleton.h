#ifndef SKELETON_H
#define SKELETON_H

#include <fstream>
#include <vector>
#include <set>
#include <sstream>
#include <cstring>

class Skeleton
{
public:

    struct Vec3
    {
        Vec3(double _x = 0.0, double _y = 0.0, double _z = 0.0)
        {
            x() = _x;
            y() = _y;
            z() = _z;
        }

        double& x() { return vec[0]; }
        double& y() { return vec[1]; }
        double& z() { return vec[2]; }

        double* data() { return vec; }
        double& operator[](int index) { return vec[index]; }

    private:
        double vec[3];
    };

    Skeleton(){}
    Skeleton(std::string skel_path)
    {
        open(skel_path);
    }

    std::vector<Vec3>  verts;
    std::vector< std::vector<int> > regions;
    std::vector< std::pair<int, int> > edges;

    bool save(std::string skel_path)
    {
        std::stringstream ss; ss << skel_path << "*";
        std::ofstream ofs(skel_path.c_str());
        std::ofstream ofsx(ss.str().c_str());

        int nv = verts.size();
        bool use_skel_format = (strstr(skel_path.c_str(), ".skel"));
        if (use_skel_format)        //else use wavefront obj format
            ofs << "SKEL" << "\n";
        else
            ofs << "# Simple OBJ skeleton file \n";

        if (use_skel_format)
            ofs << nv << " "  << edges.size() << std::endl;

        for (unsigned int i=0; i<verts.size(); ++i)
        {
            if (!use_skel_format) ofs << "v ";
            ofs << verts[i][0] << " " << verts[i][1] << " " << verts[i][2] << std::endl;

            ofsx << regions[i].size();
            for (unsigned int j=0; j< regions[i].size(); ++j)
            {
                int v_idx = regions[i][j];
                ofsx << " " << v_idx;
            }
            ofsx <<  std::endl;
        }

        for (std::vector<std::pair<int, int> > ::iterator it = edges.begin(); it!=edges.end(); ++it)
        {
            if (use_skel_format)
                ofs << "2 ";
            else
                ofs << "l ";
            ofs << (*it).first << " " << (*it).second << std::endl;
        }
        ofs.close();
        ofsx.close();
        return true;
    }

    bool open(std::string skel_path)   //SKEL or OBJ file format
    {
        std::ifstream ifs(skel_path.c_str());
        std::ifstream ifsx((skel_path+"*").c_str());

        if (ifs.is_open() && ifsx.is_open())
        {
            verts.clear();
            regions.clear();
            edges.clear();
        } else
            return false;

        bool use_skel_format = (strstr(skel_path.c_str(), ".skel"));
        if (use_skel_format)
        {
            std::string header;
            ifs >> header;
            int num_verts = 0, num_edges=0;
            ifs >> num_verts >> num_edges;
            verts.resize(num_verts);
            edges.resize(num_edges);

            for (int i=0; i<num_verts; ++i)
            {
                Vec3 v;
                ifs >> v[0] >> v[1] >> v[2];
                verts[i] = v;
            }
            for (int i=0; i<num_edges; ++i)
            {
                int vi, vj, nv;
                ifs >> nv >> vi >> vj;
                edges[i] = std::pair<int, int>(vi, vj);
            }
        } else  //OBJ file format
        {
            //TODO
        }

        regions.resize(verts.size());
        for (unsigned int i=0; i<verts.size(); ++i)
        {
            unsigned int num_v = 0;
            ifsx >> num_v;
            regions[i] = std::vector<int>(num_v, 0);
            for (unsigned int j=0; j<num_v; ++j)
                ifsx >> regions[i][j];
        }

        return true;
    }

    void compute_branches(std::vector< std::vector<int> > &branches);
    void compute_adjacency(std::vector< std::set<int> > &adjacency);
};

#endif // SKELETON_H
