//
// Created by justin on 1/11/22.
//

#ifndef SP_MER_SORTPAIRS_H
#define SP_MER_SORTPAIRS_H

#include "Utilities.h"

namespace spanners{
    using namespace std;

    priority_queue<pair<int,int>, vector<pair<int,int>>, function<bool(pair<int,int>, pair<int,int>)>> sortPairs(const vector<Point> &points){
        function<bool(pair<int,int>, pair<int,int>)> cmp = [points](pair<int,int> uv, pair<int,int> wx){
            return (getDistance(points[uv.first], points[uv.second])) > (getDistance(points[wx.first], points[wx.second]));
        };
        priority_queue<pair<int,int>, vector<pair<int,int>>, function<bool(pair<int,int>, pair<int,int>)>> temp(cmp);
        for(size_t i = 0; i < points.size(); ++i){
            for(size_t j = i + 1; j < points.size(); ++j){
                temp.push(make_pair(i,j));
            }
        }
        /*
        while(!temp.empty()){
            cout << spanners::getDistance(points[temp.top().first], points[temp.top().second]) << endl;
            temp.pop();
        }
        */
        return temp;
    }
    priority_queue<pair<int,int>, vector<pair<int,int>>, function<bool(pair<int,int>, pair<int,int>)>> sortPairsSubset(const vector<Point> &points, const vector<Point> &curPoints, const map<Point, int> &PointToIndex){
        function<bool(pair<int,int>, pair<int,int>)> cmp = [points](pair<int,int> uv, pair<int,int> wx){
            return (getDistance(points[uv.first], points[uv.second])) > (getDistance(points[wx.first], points[wx.second]));
        };
        priority_queue<pair<int,int>, vector<pair<int,int>>, function<bool(pair<int,int>, pair<int,int>)>> temp(cmp);
        for(size_t i = 0; i < curPoints.size(); ++i){
            for(size_t j = i + 1; j < curPoints.size(); ++j){
                temp.push(make_pair(PointToIndex.find(curPoints[i])->second,PointToIndex.find(curPoints[j])->second));
            }
        }
        /*
        while(!temp.empty()){
            cout << spanners::getDistance(points[temp.top().first], points[temp.top().second]) << endl;
            temp.pop();
        }
        */
        return temp;
    }

    priority_queue<pair<int,int>, vector<pair<int,int>>, function<bool(pair<int,int>, pair<int,int>)>> sortPairsSubset___(const vector<pair<Point,int>> &cellPoints){
        function<bool(pair<int,int>, pair<int,int>)> cmp = [cellPoints](pair<int,int> uv, pair<int,int> wx){
            return (getDistance(cellPoints[uv.first].first, cellPoints[uv.second].first)) > (getDistance(cellPoints[wx.first].first, cellPoints[wx.second].first));
        };
        priority_queue<pair<int,int>, vector<pair<int,int>>, function<bool(pair<int,int>, pair<int,int>)>> temp(cmp);
        for(size_t i = 0; i < cellPoints.size(); ++i){
            for(size_t j = i + 1; j < cellPoints.size(); ++j){
                temp.push(make_pair(i,j));
            }
        }
        /*
        while(!temp.empty()){
            cout << spanners::getDistance(points[temp.top().first], points[temp.top().second]) << endl;
            temp.pop();
        }
        */
        return temp;
    }
}

#endif //SP_MER_SORTPAIRS_H
