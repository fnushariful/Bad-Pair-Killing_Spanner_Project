//
// Created by justin on 1/12/22.
//

#ifndef SP_MER_FINDPERIMETER_H
#define SP_MER_FINDPERIMETER_H

#include "Utilities.h"

namespace spanners{

    typedef K::Segment_2 Segment;
    typedef K::Ray_2 Ray;

    void FindPerimeterOfCells(const vector<Point> &points, const vector<Point> &cellPoints, const vector<Edge> &edges, vector<Point> &perimeterFound){
        vector<Segment> segments;
        for(auto e : edges){
            segments.emplace_back(Segment{points[e.first], points[e.second]});
        }

        set<Point> uniquePointsOnPerimeter;

        for(auto p : cellPoints){

            Ray north(p, Point{p.x(), p.y() + 1});
            Ray south(p, Point{p.x(), p.y() - 1});
            Ray east(p, Point{p.x() + 1, p.y()});
            Ray west(p, Point{p.x() - 1, p.y()});
            bool intLeft = false;
            bool intRight = false;
            bool intUp = false;
            bool intDown = false;
            for(auto s : segments){
                if(intUp && intDown && intLeft && intRight) break;
                if(s.has_on(p)) continue;
                if(!intUp){
                    auto check = CGAL::intersection(s, north);
                    if(check){
                        intUp = true;
                    }
                }
                if(!intDown){
                    auto check = CGAL::intersection(s, south);
                    if(check){
                        intDown = true;
                    }
                }
                if(!intLeft){
                    auto check = CGAL::intersection(s, west);
                    if(check){
                        intLeft = true;
                    }
                }
                if(!intRight){
                    auto check = CGAL::intersection(s, east);
                    if(check){
                        intRight = true;
                    }
                }
            }
            if(!intUp){
                uniquePointsOnPerimeter.insert(p);
            }
            if(!intDown){
                uniquePointsOnPerimeter.insert(p);
            }
            if(!intLeft){
                uniquePointsOnPerimeter.insert(p);
            }
            if(!intRight){
                uniquePointsOnPerimeter.insert(p);
            }
        }
        for(auto p : uniquePointsOnPerimeter){
            perimeterFound.emplace_back(p);
        }
    }
}


#endif //SP_MER_FINDPERIMETER_H
