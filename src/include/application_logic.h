//
// Created by Vladyslav Yazykov on 2019-01-08.
//

#ifndef BEZIER_MAIN_H
#define BEZIER_MAIN_H

#include "forward_list"
#include "BazicPoint.h"
#include "Polygon.h"

#define MAX_POINTS 10
#define DEFAULT_POLYGONS_COUNT 5000

/// Generate random polygons, find their convex hulls and see if they intersect
/// \return Total number of intersections
std::vector<Polygon> random_polygons(size_t, size_t);

/// Get curves intersections
void get_intersections(const std::vector<Polygon> &);

/// Get curve from input
/// \return Bezier curve
Polygon get_polygon();

#endif //BEZIER_MAIN_H
