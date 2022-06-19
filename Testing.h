//
// Created by justin on 1/12/22.
//

#ifndef SP_MER_TESTING_H
#define SP_MER_TESTING_H

#include "Utilities.h"
#include "point_partition.h"
#include "ShortestPaths.h"
#include "GreedySpanner.h"
#include "GraphPrinter.h"
#include "PointGenOptions.h"
#include "FindPerimeter.h"
#include "FindNeighbors.h"
#include "MergeNeighboringCells.h"
#include "Modified_StretchFactorExperimental.h"
#include "StretchFactorExact.h"
#include "WSPD_TSpanner.h"
#include "WSPD.h"
#include "DistanceMerging.h"
#include "PerimeterKNearestNeighbor.h"

namespace spanners{
    using namespace std;
    void TestingGreedyMergeWithWSPD(){

        vector<Point> points;
        GetPoints(points);
        map<Point,int> PointToIndex;
        map<int,Point> IndicesToPoints;

        for(size_t i = 0; i < points.size(); ++i){
            PointToIndex.insert(make_pair(points[i], i));
            IndicesToPoints.insert(make_pair(i,points[i]));
        }

        pair<number_t, number_t> dimensions = std::make_pair(0,0);
        pair<number_t, number_t> adjustments = std::make_pair(0,0);

        spanners::CellToPointsMap cells;
        int cellSize = 0;
        cout << "Cell Size: ";
        cin >> cellSize;
        double t = 0.0;
        cout << "t val: ";
        cin >> t;
        cout << "s val (0 for WSPD t-spanner formula): ";
        double s;
        cin >> s;
        int KNearestNeighbor = 0;
        cout<<"Layer from Perimeter : ";
        cin>>KNearestNeighbor;
        bool fg;
        cout << "Original(0) or FG(1): ";
        cin >> fg;

        spanners::point_partition(points, cells, dimensions, adjustments, cellSize);

        map<int, vector<int>> adjMap;

        if(!fg){
            for(auto &c : cells){
                cout << "Greedy on cell " << c.first.first << " , " << c.first.second << " for " << c.second.pointsInsideTheCell.size() << endl;
                GreedySpannerConstForCells(points, c.second.pointsInsideTheCell, t, c.second.edgesWithinCell, adjMap, PointToIndex);
            }
        }else{
            for(auto &c : cells){
                cout << "FG_Greedy on cell " << c.first.first << " , " << c.first.second << " for " << c.second.pointsInsideTheCell.size() << endl;
                FG_GreedySpannerConstForCells(points, c.second.pointsInsideTheCell, t, c.second.edgesWithinCell, adjMap, PointToIndex);
            }
        }

        map<pair<long,long>, vector<Point>> perimetersOfCells;
        for(auto &c : cells){
            vector<Point> findPerimeter;
            FindPerimeterOfCells(points, c.second.pointsInsideTheCell, c.second.edgesWithinCell, findPerimeter);
            //cout<<"before perimeter of cell size "<<perimetersOfCells[c.first].size()<<endl;
            perimetersOfCells.insert(make_pair(c.first, findPerimeter));
            //cout<<"after perimeter of cell size "<<perimetersOfCells[c.first].size()<<endl;
        }

        GraphPrinter printer("./", "article");
        cout<<"Before K "<<perimetersOfCells.size()<<endl;

        perimetersOfCells = PerimeterKNearestNeighbor(KNearestNeighbor,cells,adjMap,perimetersOfCells,PointToIndex,IndicesToPoints,printer);


        cout<<"After K "<<perimetersOfCells.size()<<endl;
        vector<Point> totalPerimeter;

        for(auto pc : perimetersOfCells){
            for(auto p : pc.second){
                totalPerimeter.emplace_back(p);
            }
        }

        vector<Edge> totalEdges;
        for(const auto &c : cells){
            for(auto e : c.second.edgesWithinCell){
                totalEdges.emplace_back(e);
            }
        }

        vector<pair<longIntPair,longIntPair>> contiguousCellsToBeMerged;

        FindNeighborsOfCells(cells, contiguousCellsToBeMerged);

        vector<Edge> addedFromMerging;

        //contiguousCellsToBeMerged.clear();

        for(auto n : contiguousCellsToBeMerged){
            cout << "Merging " << n.first.first << " , " << n.first.second << "  <-->  " << n.second.first << " , " << n.second.second << endl;
            vector<Edge> temp;
            MergeNeighborsWithGreedyPerimeter(points,
                                              cells.find(n.first)->second.pointsInsideTheCell,
                                              cells.find(n.second)->second.pointsInsideTheCell,
                                              perimetersOfCells.find(n.first)->second,
                                              perimetersOfCells.find(n.second)->second,
                                              t, adjMap, PointToIndex, temp);
            for(auto e : temp){
                addedFromMerging.emplace_back(e);
                totalEdges.emplace_back(e);
            }
        }

        vector<Edge> addedWithDelBFS;

        vector<Point> leaderPoints;

        for(auto c : cells){
            leaderPoints.emplace_back(c.second.leader);
        }

        if(s == 0){
            s = 4*(t+1)/(t-1);
        }
        WellSepPairDec wspd(s, leaderPoints);
        vector<Edge> wspdEdges;
        WSPD_TSpannersConst(wspd.pairs, adjMap, PointToIndex, wspdEdges);

        //wspdEdges.clear();

        for(const auto &e : wspdEdges){
            totalEdges.emplace_back(e);
        }

        vector<pair<longIntPair, longIntPair>> distanceMergedCells;

        distanceMerging(wspdEdges,
                        dimensions,
                        adjustments,
                        t,
                        IndicesToPoints,
                        distanceMergedCells);

        //distanceMergedCells.clear();

        cout << "Distance Pair Size: " << distanceMergedCells.size() << endl;

        for(auto n : distanceMergedCells){
            cout << "Distance Merging " << n.first.first << " , " << n.first.second << "  <-->  " << n.second.first << " , " << n.second.second << endl;
            vector<Edge> temp;
            MergeNeighborsWithGreedyPerimeter(points,
                                              cells.find(n.first)->second.pointsInsideTheCell,
                                              cells.find(n.second)->second.pointsInsideTheCell,
                                              perimetersOfCells.find(n.first)->second,
                                              perimetersOfCells.find(n.second)->second,
                                              t, adjMap, PointToIndex, temp);
            for(const auto &e : temp){
                addedFromMerging.emplace_back(e);
                totalEdges.emplace_back(e);
            }
        }

        Modified_StretchFactorExpDijk(points.begin(),
                                      points.end(),
                                      totalEdges.begin(),
                                      totalEdges.end(),
                                      addedWithDelBFS,
                                      adjMap,
                                      PointToIndex,
                                      totalPerimeter,
                                      t);

        //addedWithDelBFS.clear();

        for(const auto &e : addedWithDelBFS){
            totalEdges.emplace_back(e);
        }

        double sf = StretchFactorDijkstraReduction(points.begin(), points.end(), totalEdges.begin(), totalEdges.end());

        cout << "Points: " << points.size() << endl;
        cout << "Cell size: " << cellSize << endl;
        cout << "Layer from Perimeter: "<<KNearestNeighbor<<endl;
        cout << "T val: " << t << endl;
        cout << "SF: " << sf << endl;
        cout << "Edges: " << totalEdges.size() << endl;
        cout << "M/N: " << double(totalEdges.size()) / points.size() << endl;

        spanners::GraphPrinter::OptionsList vert = {
                {"circle",     ""},
                {"vertex",     (to_string(0.05))}, // vertex width
                {"color",      "4334eb"}, // text color
                {"fill",       "4334eb"}, // vertex color
                {"line width", (std::to_string(0))} // vertex border (same color as text)
        };


        printer.autoscale(points.begin(), points.end());
        printer.drawEdges(totalEdges.begin(), totalEdges.end(), points);
        //printer.drawEdges(addedFromMerging.begin(), addedFromMerging.end(), points);
        printer.drawVertices(totalPerimeter.begin(), totalPerimeter.end(), vert);
        //printer.drawVertices(perimetersOfCells.begin(),perimetersOfCells.end(),vert);
        printer.display();
    }
}

#endif //SP_MER_TESTING_H
