//
// Created by Vladyslav Yazykov on 29/12/2018.
//

#include "Bezier.h"
#include "Polygon.h"

#define RECURSION_LIMIT 5

unsigned intersections(const Bezier<Point> &a, const Bezier<Point> &b, size_t recursion_step) {
    OUT << "Checking intersections. Recursion step: " << recursion_step << ENDL;

    // To calculate intersections we try to figure out if the convex hulls of the curves intersect.
    // If hulls do not intersect, the algorithm is stopped.
    //
    // Otherwise we subdivide the curves using De Casteljau's algorithm
    // https://en.wikipedia.org/wiki/De_Casteljau%27s_algorithm
    // for as much as precision/recursion depth allows.

    // Convex hull of a curve is given by its control points

    // If polygons do not intersect, then curves also do not intersect
    if (!Polygon(a).intersects(Polygon(b))) return 0;

    // Within this precision curves do intersect
    if (recursion_step == RECURSION_LIMIT) return 1;

    // Otherwise subdivide curves in half
    auto a_subdivided{a.subdivide()};
    auto b_subdivided{b.subdivide()};

    // And count their intersections
    return intersections(a_subdivided.first, b_subdivided.first, recursion_step + 1)
           + intersections(a_subdivided.first, b_subdivided.second, recursion_step + 1)
           + intersections(a_subdivided.second, b_subdivided.first, recursion_step + 1)
           + intersections(a_subdivided.second, b_subdivided.second, recursion_step + 1);
}
