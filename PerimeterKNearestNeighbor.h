//
// Created by shariful on 1/18/22.
//
#include "bits/stdc++.h"

#include "Utilities.h"
#include "point_partition.h"
#include "GraphPrinter.h"
#include "LatexPrinter.h"

#ifndef SP_MER_PERIMETERKNEARESTNEIGHBOR_H
#define SP_MER_PERIMETERKNEARESTNEIGHBOR_H

namespace spanners {
    using namespace std;

    map<pair<long, long>, vector<Point>>
    PerimeterKNearestNeighbor(int KNeartestNeighbor, spanners::CellToPointsMap cells, map<int, vector<int>> adjMap,
                              map<pair<long, long>, vector<Point>> perimetersOfCells, map<Point, int> PointToIndex,
                              map<int, Point> IndicesToPoints, GraphPrinter printer) {
        //GraphPrinter printer("./", "article");
        for (auto &c: cells) {
            vector<Point> perimeterPoint = perimetersOfCells[c.first];
            vector<Point> currentLayerPoint = perimetersOfCells[c.first];
            vector<Point> consecutiveTwoLayerPoint = perimetersOfCells[c.first];
            vector<Point> perimeterPointwithKNeighbor = perimetersOfCells[c.first];

            int NeighborCnt = KNeartestNeighbor;
            while ( NeighborCnt > 0 ) {
                set<Point> nextlayerPoint;
                for (auto p: currentLayerPoint) {
                    Point tmp = Point(p.x(), p.y());
                    vector<int> tmpadjPoint = adjMap[PointToIndex[tmp]];
                    vector<pair<double, Point>> adjPoint;

                    number_t u_x = p.x();
                    number_t u_y = p.y();

                    coords u_adj = std::make_pair(u_x, u_y);

                    double adjDistance;

                    for (auto v: tmpadjPoint) {
                        auto vv = IndicesToPoints[v];

                        Point nextPoint = Point(vv.x(), vv.y());
                        number_t v_x = vv.x();
                        number_t v_y = vv.y();
                        coords v_adj = std::make_pair(v_x, v_y);
                        adjDistance = distance(u_adj, v_adj);

                        adjPoint.emplace_back(adjDistance, nextPoint);
                    }

                    sort(adjPoint.begin(), adjPoint.end());

                    for (auto x: adjPoint) {
                        if (std::find(consecutiveTwoLayerPoint.begin(), consecutiveTwoLayerPoint.end(), x.second) !=
                                consecutiveTwoLayerPoint.end())
                            continue;
                        else {
                            nextlayerPoint.insert(x.second);
                        }
                    }
                }
                copy(nextlayerPoint.begin(),nextlayerPoint.end(),back_inserter(consecutiveTwoLayerPoint));
                currentLayerPoint.clear();
                copy(nextlayerPoint.begin(), nextlayerPoint.end(), back_inserter(currentLayerPoint));
                copy(nextlayerPoint.begin(), nextlayerPoint.end(), back_inserter(perimeterPointwithKNeighbor));
                nextlayerPoint.clear();
                NeighborCnt--;
            }
            perimetersOfCells.erase(c.first);
            perimetersOfCells.insert(make_pair(c.first, perimeterPointwithKNeighbor));
        }
        return perimetersOfCells;
    }
}


#endif //SP_MER_PERIMETERKNEARESTNEIGHBOR_H
