//
// Created by justin on 1/12/22.
//

#ifndef SP_MER_FINDNEIGHBORS_H
#define SP_MER_FINDNEIGHBORS_H

#include "Utilities.h"
#include "point_partition.h"

namespace spanners{
    void FindNeighborsOfCells(CellToPointsMap cells, vector<pair<longIntPair,longIntPair>> &neighbors){
        for(auto c : cells){
            if(c.second.neighbors[0] != standard){
                neighbors.emplace_back(c.first, c.second.neighbors[0]);
            }
            if(c.second.neighbors[2] != standard){
                neighbors.emplace_back(c.first, c.second.neighbors[2]);
            }
            if( c.second.neighbors[1] != standard &&
                c.second.neighbors[0] == standard &&
                c.second.neighbors[2] == standard){
                neighbors.emplace_back(c.first, c.second.neighbors[1]);
            }
            if( c.second.neighbors[3] != standard &&
                c.second.neighbors[2] == standard &&
                c.second.neighbors[4] == standard){
                neighbors.emplace_back(c.first, c.second.neighbors[3]);
            }
        }
    }
}

#endif //SP_MER_FINDNEIGHBORS_H
