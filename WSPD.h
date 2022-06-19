//
// Created by justin on 1/12/22.
//

#ifndef SP_MER_WSPD_H
#define SP_MER_WSPD_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <queue>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;

namespace spanners {
    using namespace std;

    struct Pair {
        vector<Point> Sv, Sw;
    };

    class SplitTree {
    public:
        double boundaries[4];
        SplitTree *s1, *s2;
        vector<Point> pSet;
        CGAL::Bbox_2 boundingBox;

        explicit SplitTree(const vector<Point> &points);
    };

    class WellSepPairDec {
    public:
        double s;
        SplitTree *root;
        vector<Pair> pairs;

        WellSepPairDec(double s, const vector<Point> &points);

        void InsertInternalPairs(SplitTree *root, queue<Pair> &q);

        void ExtractPairs(SplitTree *root, vector<Pair> &pairs);

        bool CalcS(double s, Pair in);

        void InsertNonWellSeparatedPairs(Pair cur, queue<Pair> &q);
    };
    bool WellSepPairDec::CalcS(double s, Pair in) {

        CGAL::Bbox_2 boxV = CGAL::bbox_2(in.Sv.begin(), in.Sv.end());
        CGAL::Bbox_2 boxW = CGAL::bbox_2(in.Sw.begin(), in.Sw.end());
        double vx, vy, wx, wy;

        double vx_min = boxV.xmin();
        double vx_max = boxV.xmax();
        double vy_min = boxV.ymin();
        double vy_max = boxV.ymax();

        double wx_min = boxW.xmin();
        double wx_max = boxW.xmax();
        double wy_min = boxW.ymin();
        double wy_max = boxW.ymax();

        vx = vx_max - vx_min;
        vy = vy_max - vy_min;
        wx = wx_max - wx_min;
        wy = wy_max - wy_min;

        Point centerV{vx_min + vx/2, vy_min + vy/2};
        Point centerW{wx_min + wx/2, wy_min + wy/2};

        Point lowerV (vx_min, vy_min);
        Point lowerW (wx_min, wy_min);

        double diagonalV = sqrt(CGAL::squared_distance(lowerV, centerV));
        double diagonalW = sqrt(CGAL::squared_distance(lowerW, centerW));

        double radius = max(diagonalV, diagonalW);
        double distance = sqrt(CGAL::squared_distance(centerV, centerW)) - 2*radius;
        return distance >= (s*radius);

        /* ACCOUNT FOR SHIFTING CIRCLE THAT ENCOMPASSES SMALLER BOUNDING BOX

        Point largerCenter = (diagonalV > diagonalW) ? centerV : centerW;
        Point smallerCenter = (diagonalV > diagonalW) ? centerW : centerV; // verify correctness
        Point smallerLower = (diagonalV > diagonalW) ? lowerW : lowerV; // verify correctness
        double smallerDiagonal = (diagonalV > diagonalW) ? diagonalW : diagonalV;

        double l = radius - smallerDiagonal;

        double smallerY = smallerCenter.y() - smallerLower.y();
        double smallerX = smallerCenter.x() - smallerLower.x();

        double theta = atan(smallerY/smallerX);

        vector<Point> smallCenters;

        smallCenters.push_back(Point(smallerCenter.x() - l*cos(theta), smallerCenter.y() - l*sin(theta)));
        smallCenters.push_back(Point(smallerCenter.x() - l*cos(theta), smallerCenter.y() + l*sin(theta)));
        smallCenters.push_back(Point(smallerCenter.x() + l*cos(theta), smallerCenter.y() - l*sin(theta)));
        smallCenters.push_back(Point(smallerCenter.x() + l*cos(theta), smallerCenter.y() + l*sin(theta)));

        for (auto center : smallCenters) {

            double centerDistance = sqrt(CGAL::squared_distance(center, largerCenter));
            double shortestDistance = centerDistance - 2 * radius;

            if (shortestDistance > s*radius)
                return true;

        } */

        return false;

    }

    WellSepPairDec::WellSepPairDec(double s, const vector<Point> &points){
        this->root = new SplitTree(points);
        this->s = s;
        ExtractPairs(this->root, this->pairs);
    }

    void WellSepPairDec::InsertInternalPairs(SplitTree *rootOfSplit, queue<Pair> &q) {
        SplitTree *iter = rootOfSplit;
        if(iter->s1 != nullptr && iter->s2 != nullptr){
            q.push(Pair{iter->s1->pSet, iter->s2->pSet});
            InsertInternalPairs(iter->s1, q);
            InsertInternalPairs(iter->s2, q);
        }else if(iter->s1 != nullptr && iter->s2 == nullptr){
            InsertInternalPairs(iter->s1, q);
        }else if(iter->s1 == nullptr && iter->s2 != nullptr){
            InsertInternalPairs(iter->s2, q);
        }else return;
    }

    void WellSepPairDec::ExtractPairs(SplitTree *root, vector<Pair> &pairs){
        queue<Pair> q;
        InsertInternalPairs(this->root, q);
        while(!q.empty()){
            Pair cur = q.front();
            q.pop();
            if(CalcS(this->s, cur)){
                this->pairs.emplace_back(cur);
            }else{
                InsertNonWellSeparatedPairs(cur, q);
            }
        }
        cout << "q empty\n";
    }

    void WellSepPairDec::InsertNonWellSeparatedPairs(Pair cur, queue<Pair> &q) {
        CGAL::Bbox_2 boxSv = CGAL::bbox_2(cur.Sv.begin(), cur.Sv.end());
        CGAL::Bbox_2 boxSw = CGAL::bbox_2(cur.Sw.begin(), cur.Sw.end());
        double dxSv, dySv, dxSw, dySw;
        dxSv = boxSv.xmax() - boxSv.xmin();
        dySv = boxSv.ymax() - boxSv.ymin();
        dxSw = boxSw.xmax() - boxSw.xmin();
        dySw = boxSw.ymax() - boxSw.ymin();
        vector<Point> *larger, *smaller;
        double *XL, *YL;
        double largestV, largestW;
        if(dxSv >= dySv){
            largestV = dxSv;
        }else{
            largestV = dySv;
        }
        if(dxSw >= dySw){
            largestW = dxSw;
        }else{
            largestW = dySw;
        }
        if(largestV >= largestW){
            larger = &cur.Sv;
            smaller = &cur.Sw;
            XL = &dxSv;
            YL = &dySv;
        }else{
            larger = &cur.Sw;
            smaller = &cur.Sv;
            XL = &dxSw;
            YL = &dySw;
        }
        double div;
        vector<Point> w1, w2;
        if(*XL >= *YL){
            vector<Point> temp = *larger;
            CGAL::Bbox_2 box = CGAL::bbox_2(temp.begin(), temp.end());
            div = box.xmin() + *XL / 2;
            for(auto p : *larger){
                if(p.x() <= div){
                    w1.emplace_back(p);
                }else{
                    w2.emplace_back(p);
                }
            }
        }else{
            vector<Point> temp = *larger;
            CGAL::Bbox_2 box = CGAL::bbox_2(temp.begin(), temp.end());
            div = box.ymin() + *YL / 2;
            for(auto p : *larger){
                if(p.y() <= div){
                    w1.emplace_back(p);
                }else{
                    w2.emplace_back(p);
                }
            }
        }
        q.push(Pair{*smaller, w1});
        q.push(Pair{*smaller, w2});
    }


// this has been examined
    SplitTree::SplitTree(const vector<Point> &points){
        this->pSet = points;

        if(this->pSet.size() <= 1){ // converged
            s1 = s2 = nullptr;
        }else{
            boundingBox = CGAL::bbox_2(points.begin(), points.end());
            boundaries[0] = boundingBox.xmin();
            boundaries[1] = boundingBox.xmax();
            boundaries[2] = boundingBox.ymin();
            boundaries[3] = boundingBox.ymax();
            double dy = boundingBox.ymax() - boundingBox.ymin();
            double dx = boundingBox.xmax() - boundingBox.xmin();
            vector<Point> s1Points, s2Points;
            if(dy >= dx){ // cut it in half along y
                double mid = dy/2 + boundaries[2];
                for(auto p : this->pSet){
                    if(p.y() >= mid){
                        s1Points.emplace_back(p);
                    }else{
                        s2Points.emplace_back(p);
                    }
                }
            }else{ // cut it in half along x
                double mid = dx/2 + boundaries[0];
                for(auto p : this->pSet){
                    if(p.x() >= mid){
                        s1Points.emplace_back(p);
                    }else{
                        s2Points.emplace_back(p);
                    }
                }
            }
            if(s1Points.empty()){
                this->s1 = nullptr;
            }else{
                this->s1 = new SplitTree(s1Points);
            }
            if(s2Points.empty()){
                this->s2 = nullptr;
            }else{
                this->s2 = new SplitTree(s2Points);
            }
        }
    }
}

#endif //SP_MER_WSPD_H
