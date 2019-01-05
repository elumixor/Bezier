//
// Created by Vladyslav Yazykov on 29/12/2018.
//

#include "Bezier.h"
#include "Polygon.h"

unsigned intersections(const Bezier<Point> &a, const Bezier<Point> &b) {
    // To calculate intersections we try to figure out if the convex hulls of the curves intersect.
    // If hulls do not intersect, the algorithm is stopped.
    //
    // Otherwise we subdivide the curves using De Casteljau's algorithm
    // https://en.wikipedia.org/wiki/De_Casteljau%27s_algorithm
    // for as much as precision/recursion depth allows.

    // Convex hull of a curve is given by its control points
    Polygon hull_a{a.C, a.n + 1};
    Polygon hull_b{b.C, a.n + 1};

//    if ()

    return 0;
}
