//
// Created by justin on 1/11/22.
//

#ifndef SP_MER_GREEDYSPANNER_H
#define SP_MER_GREEDYSPANNER_H

#include "Utilities.h"
#include "ShortestPaths.h"
#include "SortPairs.h"

namespace spanners{
    void GreedySpannerConst(const vector<Point> &points, double t, vector<Edge> &edges, map<int, vector<int>> &adjMap){
        priority_queue<pair<int,int>, vector<pair<int,int>>, function<bool(pair<int,int>, pair<int,int>)>> pairs = sortPairs(points);
        while(!pairs.empty()){
            if(shortestPaths(points, pairs.top(), adjMap) > t * getDistance(points[pairs.top().first], points[pairs.top().second])){
                edges.emplace_back(make_pair(pairs.top().first, pairs.top().second));
                adjMap[pairs.top().first].emplace_back(pairs.top().second);
                adjMap[pairs.top().second].emplace_back(pairs.top().first);
            }
            pairs.pop();
        }
    }
    void GreedySpannerConstForCells(const vector<Point> &points, const vector<Point> &curPoints, double t, vector<Edge> &edges, map<int, vector<int>> &adjMap, const map<Point,int> &PointToIndex){
        priority_queue<pair<int,int>, vector<pair<int,int>>, function<bool(pair<int,int>, pair<int,int>)>> pairs = sortPairsSubset(
                points, curPoints, PointToIndex);
        while(!pairs.empty()){
            if(shortestPaths(points, pairs.top(), adjMap) > t * getDistance(points[pairs.top().first], points[pairs.top().second])){
                edges.emplace_back(make_pair(pairs.top().first, pairs.top().second));
                adjMap[pairs.top().first].emplace_back(pairs.top().second);
                adjMap[pairs.top().second].emplace_back(pairs.top().first);
            }
            pairs.pop();
        }
    }

    void FG_GreedySpannerConstForCells(const vector<Point> &points, const vector<Point> &curPoints, double t, vector<Edge> &edges, map<int,
            vector<int>> &adjMap, const map<Point,int> &PointToIndex){


        if(curPoints.size() == 1) return;

        vector<vector<double>> distances(curPoints.size());
        for(auto &x : distances){
            x.resize(curPoints.size());
            for(size_t i = 0; i < curPoints.size(); ++i){
                x[i] = (DBL_MAX);
            }
        }

        map<int,int> localIndex;
        for(size_t i = 0; i < curPoints.size(); ++i){
            localIndex.insert(make_pair(PointToIndex.find(curPoints[i])->second, i));
        }

        priority_queue<pair<int,int>, vector<pair<int,int>>, function<bool(pair<int,int>, pair<int,int>)>> pairs = sortPairsSubset(
                points, curPoints, PointToIndex);

        while(!pairs.empty()){
            double dist_u_to_v = 0.0;
            if(distances[localIndex.find(pairs.top().first)->second][localIndex.find(pairs.top().second)->second]
                > t * getDistance(points[pairs.top().first], points[pairs.top().second])){
                dist_u_to_v = shortestPathsForFG(points, pairs.top(), adjMap, distances, localIndex);
            }
            if(dist_u_to_v > t * getDistance(points[pairs.top().first], points[pairs.top().second])){
                edges.emplace_back(make_pair(pairs.top().first, pairs.top().second));
                adjMap[pairs.top().first].emplace_back(pairs.top().second);
                adjMap[pairs.top().second].emplace_back(pairs.top().first);
            }
            pairs.pop();
        }
    }

    void FG_GreedySpannerConstForCells_V2(const vector<Point> &points, const vector<Point> &curPoints, double t, vector<Edge> &edges, map<int,
            vector<int>> &adjMap, const map<Point,int> &PointToIndex){

        if(curPoints.size() == 1) return;

        vector<vector<double>> distances(curPoints.size());

        for(auto &x : distances){
            x.resize(curPoints.size(), DBL_MAX);
        }

        typedef pair<Point,int> localPoints;
        vector<localPoints> locals;

        for(size_t i = 0; i < curPoints.size(); ++i){
            locals.emplace_back(curPoints[i], i);
        }

        priority_queue<pair<int,int>, vector<pair<int,int>>, function<bool(pair<int,int>, pair<int,int>)>> pairs = sortPairsSubset___(locals);

        while(!pairs.empty()){
            double dist_u_to_v = 0.0;
            if(distances[pairs.top().first][pairs.top().second]
               > t * getDistance(curPoints[pairs.top().first], curPoints[pairs.top().second])){
                dist_u_to_v = shortestPathsForFG___(locals, pairs.top(), adjMap, distances);
            }
            if(dist_u_to_v > t * getDistance(curPoints[pairs.top().first], curPoints[pairs.top().second])){
                edges.emplace_back(make_pair(PointToIndex.find(curPoints[pairs.top().first])->second, PointToIndex.find(curPoints[pairs.top().second])->second));
                adjMap[PointToIndex.find(curPoints[pairs.top().first])->second].emplace_back(PointToIndex.find(curPoints[pairs.top().second])->second);
                adjMap[PointToIndex.find(curPoints[pairs.top().second])->second].emplace_back(PointToIndex.find(curPoints[pairs.top().first])->second);
            }
            pairs.pop();
        }
    }
}

#endif //SP_MER_GREEDYSPANNER_H
