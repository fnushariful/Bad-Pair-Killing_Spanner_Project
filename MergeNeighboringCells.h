//
// Created by justin on 1/12/22.
//

#ifndef SP_MER_MERGENEIGHBORINGCELLS_H
#define SP_MER_MERGENEIGHBORINGCELLS_H

#include "Utilities.h"
#include "SortPairs.h"
#include "ShortestPaths.h"

namespace spanners{
    void MergeNeighborsWithGreedyPerimeter(const vector<Point> &points, const vector<Point> &cellPointsOne, const vector<Point> &cellPointsTwo,
                                           const vector<Point> &cellPerimeterOne, const vector<Point> &cellPerimeterTwo,
                                           double t, map<int,vector<int>> &adjMap, const map<Point,int> &PointToIndex,
                                           vector<Edge> &added){

        vector<Point> perimeters;
        for(auto p : cellPerimeterOne){
            perimeters.emplace_back(p);
        }
        for(auto p : cellPerimeterTwo){
            perimeters.emplace_back(p);
        }

        set<int> currentPoints;
        for(auto p : cellPointsOne){
            currentPoints.insert(PointToIndex.find(p)->second);
        }
        for(auto p : cellPointsTwo){
            currentPoints.insert(PointToIndex.find(p)->second);
        }

        priority_queue<pair<int,int>, vector<pair<int,int>>, function<bool(pair<int,int>, pair<int,int>)>> pairs = sortPairsSubset(points, perimeters, PointToIndex);

        while(!pairs.empty()){
            if(shortestPathsBetweenTwoCells(points, currentPoints, pairs.top(), adjMap) > t * getDistance(points[pairs.top().first], points[pairs.top().second])){
                added.emplace_back(make_pair(pairs.top().first, pairs.top().second));
                adjMap[pairs.top().first].emplace_back(pairs.top().second);
                adjMap[pairs.top().second].emplace_back(pairs.top().first);
            }
            pairs.pop();
        }
    }
}

#endif //SP_MER_MERGENEIGHBORINGCELLS_H
