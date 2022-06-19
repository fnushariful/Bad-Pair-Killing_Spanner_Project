//
// Created by justin on 1/12/22.
//

#ifndef SP_MER_WSPD_TSPANNER_H
#define SP_MER_WSPD_TSPANNER_H

#include "Utilities.h"
#include "WSPD.h"

namespace spanners{
    void WSPD_TSpannersConst(const vector<Pair> &pairs, map<int,vector<int>> &adjMap, const map<Point, int> &PointToIndices, vector<spanners::Edge> &edges){
        for(auto p : pairs){
            adjMap[PointToIndices.find(p.Sv[0])->second].emplace_back(PointToIndices.find(p.Sw[0])->second);
            adjMap[PointToIndices.find(p.Sw[0])->second].emplace_back(PointToIndices.find(p.Sv[0])->second);
            edges.emplace_back(PointToIndices.find(p.Sv[0])->second, PointToIndices.find(p.Sw[0])->second);
        }
    }
}

#endif //SP_MER_WSPD_TSPANNER_H
