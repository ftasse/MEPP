#include <queue>
#include <set>
#include <iostream>

#include "skeleton.h"

void Skeleton::compute_adjacency(std::vector< std::set<int> > &adjacency)
{
    adjacency.resize(verts.size());

    for (unsigned int i=0; i<edges.size(); ++i)
    {
        adjacency[edges[i].first].insert(edges[i].second);
        adjacency[edges[i].second].insert(edges[i].first);
    }
}

void Skeleton::compute_branches(std::vector< std::vector<int> > &branches)
{
    branches.clear();
    std::vector< bool > masked(verts.size(), false);
    std::vector< std::set<int> > adjacency;
    compute_adjacency(adjacency);

    for (unsigned int i=0; i<edges.size(); ++i)
    {
        adjacency[edges[i].first].insert(edges[i].second);
        adjacency[edges[i].second].insert(edges[i].first);
    }

    std::queue< std::pair<int, int> > start_segs;
    for (unsigned int i =0; i<adjacency.size(); ++i)
    {
        if (adjacency[i].size() !=2 )
        {
            masked[i] = true;
            for (std::set<int>::iterator it = adjacency[i].begin(); it != adjacency[i].end(); ++it)
            {
                start_segs.push(std::pair<int, int>(i, *it));
            }
        }
    }

    while (!start_segs.empty())
    {
        std::pair<int, int> cur = start_segs.front();
        start_segs.pop();

        if (adjacency[cur.second].size() == 2 && masked[cur.second])
            continue;

        std::vector<int> branch;
        branch.push_back(cur.first);
        while (adjacency[cur.second].size() == 2)
        {
            branch.push_back(cur.second);
            masked[cur.second] = true;
            std::set<int>::iterator it = adjacency[cur.second].begin();
            if (*it == cur.first)   ++it;
            cur.first = cur.second;
            cur.second = *it;
        }
        branch.push_back(cur.second);

        if (!masked[cur.second])
        {
            masked[cur.second] = true;
            for (std::set<int>::iterator it = adjacency[cur.second].begin(); it != adjacency[cur.second].end(); ++it)
            {
                if (!masked[*it])
                    start_segs.push(std::pair<int, int>(cur.second, *it));
            }
        }

        branches.push_back(branch);

        /*for (unsigned int k=0; k<branch.size(); ++k)
            std::cout << branch[k] << " ";
        std::cout << "\n" << std::flush;*/
    }
}
