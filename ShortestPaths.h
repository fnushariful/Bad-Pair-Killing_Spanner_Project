//
// Created by justin on 1/11/22.
//

#ifndef SP_MER_SHORTESTPATHS_H
#define SP_MER_SHORTESTPATHS_H

#include "Utilities.h"

namespace spanners{
    using namespace std;
   double shortestPaths(const vector<Point> &points, pair<int,int> cur, map<int,vector<int>> adjMap){
       //check if current pair are adjacent
       //in order to bypass dijkstra
       auto adj_list = adjMap[cur.first];
       for(auto a : adjMap[cur.first]){
           if(a == cur.second){
               return getDistance(points[cur.first], points[cur.second]);
           }
       }
       vector<double> distances(points.size(), DBL_MAX);
       distances[cur.first] = 0.0;
       priority_queue<pair<double,int>, vector<pair<double,int>>, greater<>> pq;
       pq.push(make_pair(0.0, cur.first));
       while(!pq.empty()){
           int pos = pq.top().second;
           pq.pop();
           auto adj = adjMap[pos];
           for(auto a : adj){
               double pathTotal_with_eucDistToAdj = distances[pos] + getDistance(points[pos], points[a]);
               if(distances[a] > pathTotal_with_eucDistToAdj){
                   distances[a] = pathTotal_with_eucDistToAdj;
                   pq.push(make_pair(distances[a], a));
               }
           }
       }
       return distances[cur.second];
   }

    double shortestPathsForFG(const vector<Point> &points, pair<int,int> cur, map<int,vector<int>> &adjMap,
                            vector<vector<double>> &distances, const map<int,int> &localIndex){
        //check if current pair are adjacent
        //in order to bypass dijkstra
        for(auto a : adjMap[cur.first]){
            if(a == cur.second){
                double dist = getDistance(points[cur.first], points[cur.second]);
                distances[localIndex.find(cur.first)->second][localIndex.find(cur.second)->second] = dist;
                distances[localIndex.find(cur.second)->second][localIndex.find(cur.first)->second] = dist;
                return dist;
            }
        }
        vector<double> local_distances(points.size(), DBL_MAX);
        local_distances[cur.first] = 0.0;
        priority_queue<pair<double,int>, vector<pair<double,int>>, greater<>> pq;
        pq.push(make_pair(0.0, cur.first));
        while(!pq.empty()){
            int pos = pq.top().second;
            pq.pop();
            auto adj = adjMap[pos];
            for(auto a : adj){
                double pathTotal_with_eucDistToAdj = local_distances[pos] + getDistance(points[pos], points[a]);
                if(local_distances[a] > pathTotal_with_eucDistToAdj){
                    local_distances[a] = pathTotal_with_eucDistToAdj;
                    pq.push(make_pair(local_distances[a], a));
                }
            }
        }

        for(size_t i = 0; i < local_distances.size(); ++i){
            if(local_distances[i] == 0) continue;
            distances[localIndex.find(cur.first)->second][localIndex.find(i)->second] = local_distances[i];
            distances[localIndex.find(i)->second][localIndex.find(cur.first)->second] = local_distances[i];
        }

        return local_distances[cur.second];

    }

    double shortestPathsForFG___(const vector<pair<Point,int>> &points, pair<int,int> cur,
                                 map<int,vector<int>> &adjMap, vector<vector<double>> &distances){
        //check if current pair are adjacent
        //in order to bypass dijkstra
        auto adj_list = adjMap[cur.first];
        for(auto a : adj_list){
            if(a == cur.second){
                double dist = getDistance(points[cur.first].first, points[cur.second].first);
                distances[cur.first][cur.second] = dist;
                distances[cur.second][cur.first] = dist;
                return dist;
            }
        }
        vector<double> local_distances(points.size(), DBL_MAX);
        local_distances[cur.first] = 0.0;
        priority_queue<pair<double,int>, vector<pair<double,int>>, greater<>> pq;
        pq.push(make_pair(0.0, cur.first));
        while(!pq.empty()){
            int pos = pq.top().second;
            pq.pop();
            auto adj = adjMap[pos];
            for(auto a : adj){
                double pathTotal_with_eucDistToAdj = local_distances[pos] + getDistance(points[pos].first, points[a].first);
                if(local_distances[a] > pathTotal_with_eucDistToAdj){
                    local_distances[a] = pathTotal_with_eucDistToAdj;
                    pq.push(make_pair(local_distances[a], a));
                }
            }
        }

        for(size_t i = 0; i < local_distances.size(); ++i){
            if(cur.first == i) continue;
            distances[cur.first][i] = local_distances[i];
            distances[i][cur.first] = local_distances[i];
        }

        return local_distances[cur.second];

    }

    double shortestPathsForIndividualCells(const vector<Point> &points, const vector<Point> &curPoints, pair<int,int> cur, map<int,vector<int>> adjMap){
        //check if current pair are adjacent
        //in order to bypass dijkstra
        auto adj_list = adjMap[cur.first];
        for(auto a : adjMap[cur.first]){
            if(a == cur.second){
                return getDistance(points[cur.first], points[cur.second]);
            }
        }
        vector<double> distances(points.size(), DBL_MAX);
        distances[cur.first] = 0.0;
        priority_queue<pair<double,int>, vector<pair<double,int>>, greater<>> pq;
        pq.push(make_pair(0.0, cur.first));
        while(!pq.empty()){
            int pos = pq.top().second;
            pq.pop();
            auto adj = adjMap[pos];
            for(auto a : adj){
                double pathTotal_with_eucDistToAdj = distances[pos] + getDistance(points[pos], points[a]);
                if(distances[a] > pathTotal_with_eucDistToAdj){
                    distances[a] = pathTotal_with_eucDistToAdj;
                    pq.push(make_pair(distances[a], a));
                }
            }
        }
        return distances[cur.second];
    }
    double shortestPathsBetweenTwoCells(const vector<Point> &points, const set<int> &currentPoints,
                                        pair<int,int> cur, map<int,vector<int>> &adjMap){

        vector<double> distances(points.size(), DBL_MAX);
        distances[cur.first] = 0.0;

        priority_queue<pair<double,int>, vector<pair<double,int>>, greater<>> pq;
        pq.push(make_pair(0.0, cur.first));

        while(!pq.empty()){

            int pos = pq.top().second;
            pq.pop();

            auto adj = adjMap[pos];
            for(auto a : adj){
                if(!(currentPoints.find(a) == currentPoints.end())){
                    double pathTotal_with_eucDistToAdj = distances[pos] + getDistance(points[pos], points[a]);
                    if(distances[a] > pathTotal_with_eucDistToAdj){
                        distances[a] = pathTotal_with_eucDistToAdj;
                        pq.push(make_pair(pathTotal_with_eucDistToAdj, a));
                    }
                }
            }
        }

        return distances[cur.second];

    }
}

#endif //SP_MER_SHORTESTPATHS_H
