//
// Created by lucasfuturist on 1/12/22.
//

#ifndef SPANNERS_MERGING_DISTANCEMERGING_H
#define SPANNERS_MERGING_DISTANCEMERGING_H

#include <iostream>
#include <vector>
#include <chrono>
#include <queue>
#include <limits>
#include <cmath>
#include <random>
#include <unordered_set>
#include <boost/heap/fibonacci_heap.hpp> // ordering
#include <CGAL/Bbox_2.h>

#include "Utilities.h"

namespace spanners {

    typedef pair<number_t, number_t> coords;

    inline number_t distance(coords u, coords v) {
        return sqrt( pow((u.first - v.first), 2) + pow((u.second - v.second), 2));
    }

    inline bool neighborCheck(longIntPair u, longIntPair v) {

        size_t dx = abs(u.first - v.first);
        size_t dy = abs(u.second - v.second);

        bool horizNeighbor = dx == 1 && dy == 0;
        bool vertNeighbor = dx == 0 && dy == 1;
        bool diagNeighbor = dx == 1 && dy == 1;

        return (!horizNeighbor && !vertNeighbor && !diagNeighbor);

    }

    void distanceMerging(vector<spanners::Edge> &edges, pair<number_t, number_t> &dimensions, pair<number_t, number_t> &adjustments,
                         double t, map<int, Point> &IndicesToPoints, vector<pair<longIntPair, longIntPair>> &distanceMergedCells) {

        number_t length_x = dimensions.first;
        number_t length_y = dimensions.second;
        number_t adj_x = adjustments.first;
        number_t adj_y = adjustments.second;

        for (auto edge : edges) {

            // determine grid cell coordinates of u and v in each edge

            auto index_u = edge.first;
            auto index_v = edge.second;

            auto u = IndicesToPoints[index_u];
            auto v = IndicesToPoints[index_v];

            number_t u_x = u.x() + adj_x;
            number_t u_y = u.y() + adj_y;

            number_t v_x = v.x() + adj_x;
            number_t v_y = v.y() + adj_y;

            coords u_adj = std::make_pair(u_x, u_y);
            coords v_adj = std::make_pair(v_x, v_y);
            
            // determine grid cell coordinate of u
            long box_x = floor(u_x / length_x);
            long box_y = floor(u_y / length_y);

            bool localMax_x = abs(u_x - (box_x * length_x)) < EPSILON && u_x > 0;
            bool localMax_y = abs(u_y - (box_y * length_y)) < EPSILON && u_y > 0;

            box_x -= localMax_x;
            box_y -= localMax_y;

            longIntPair u_grid = std::make_pair(box_x, box_y);

            // determine grid cell coordinate of v
            box_x = floor(v_x / length_x);
            box_y = floor(v_y / length_y);

            localMax_x = abs(v_x - (box_x * length_x)) < EPSILON && v_x > 0;
            localMax_y = abs(v_y - (box_y * length_y)) < EPSILON && v_y > 0;

            box_x -= localMax_x;
            box_y -= localMax_y;

            longIntPair v_grid = std::make_pair(box_x, box_y);

            // determine if the cells should be merged

            bool notNeighbors = neighborCheck(u_grid, v_grid);

            if (!notNeighbors)
                continue;

            long uBox_x = u_grid.first;
            long uBox_y = u_grid.second;
            long vBox_x = v_grid.first;
            long vBox_y = v_grid.first;

            vector<coords> corners_u(4);
            vector<coords> corners_v(4);

            corners_u.at(0) = std::make_pair(uBox_x * length_x, uBox_y*length_y);
            corners_u.at(1) = std::make_pair((uBox_x+1) * length_x, uBox_y*length_y);
            corners_u.at(2) = std::make_pair((uBox_x+1) * length_x, (uBox_y+1)*length_y);
            corners_u.at(3) = std::make_pair(uBox_x * length_x, (uBox_y+1)*length_y);

            corners_v.at(0) = std::make_pair(vBox_x * length_x, vBox_y*length_y);
            corners_v.at(1) = std::make_pair((vBox_x+1) * length_x, vBox_y*length_y);
            corners_v.at(2) = std::make_pair((vBox_x+1) * length_x, (vBox_y+1)*length_y);
            corners_v.at(3) = std::make_pair(vBox_x * length_x, (vBox_y+1)*length_y);

            bool farEnough = false;
            number_t leaderEdgeLength = distance(u_adj, v_adj);

            for (size_t i = 0; i < 4; i++) {

                for (size_t j = 0; j < 4; j++) {

                    coords u_corner = corners_u[i];
                    coords v_corner = corners_v[j];

                    number_t totalPath = (t*distance(u_corner, u_adj)) + leaderEdgeLength + (t*distance(v_adj, v_corner));
                    number_t cornerToCorner = distance(u_corner, v_corner);

                    farEnough = farEnough || ((totalPath / cornerToCorner) > t);
                }
            }

            if (farEnough) {
                pair<longIntPair, longIntPair> gridsToMerge = std::make_pair(u_grid, v_grid);
                distanceMergedCells.push_back(gridsToMerge);
            }

        }

        return;
    }

}

#endif //SPANNERS_MERGING_DISTANCEMERGING_H
