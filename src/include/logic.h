//
// Created by Vladyslav Yazykov on 2019-01-08.
//

#ifndef BEZIER_MAIN_H
#define BEZIER_MAIN_H

// Function defs, used in main.cpp for testing

// constants
#define DEFAULT_CURVES_COUNT 50
#define MAX_CURVE_DEGREE 10
#define RECURSION_LIMIT 5

#include "forward_list"
#include "Bezier.h"
#include "BazicPoint.h"

/// Generate random curves and see if they intersect
/// \return Total number of intersections
std::forward_list<Bezier<Point>> random_curves(size_t);

/// Get curves intersections
void get_intersections(const std::forward_list<Bezier<Point>> &);

/// Get curve from input
/// \return Bezier curve
Bezier<Point> get_curve();

/// Calculate the number of intersections with the other curve
unsigned curve_intersections(const Bezier<Point> &, const Bezier<Point> &, size_t = 0);

#endif //BEZIER_MAIN_H
