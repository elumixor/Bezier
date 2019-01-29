//
// Created by Vladyslav Yazykov on 30/12/2018.
//

#include <utility>
#include <string>
#include <map>
#include <sstream>
#include <list>
#include "Polygon.h"

/// Check if there are at least to distinct points
static bool is_dot(std::vector<Point> points) {
    for (size_t i{0}; i < points.size(); i++)
        for (size_t j{i + 1}; j < points.size(); j++)
            if (points[i] != points[j]) return false;
    return true;
}

/// Construct the polygon's convex hull from points
Polygon::Polygon(std::vector<Point> points) {
    if (points.size() < 2 || is_dot(points))
        throw std::invalid_argument("Polygon requires at least two distinct points");

    // Copy array to allow for geometrical transformations
    std::vector<Point> _points(points);

    // Sort points horizontally, then vertically
#define sort() std::sort(_points.begin(), _points.end(), [](auto a, auto b) { return a.x < b.x || (a.x == b.x && a.y < b.y); });
    sort();

    //region Get bounding points
    const Point *left{&*points.begin()}, *top{left}, *right{top}, *bottom{right};

    for (size_t i{0}; i < points.size(); i++) {
        if (points[i].x < (*left).x) left = &points[i];
        else if (points[i].x > (*right).x) right = &points[i];
        if (points[i].y < (*bottom).y) bottom = &points[i];
        else if (points[i].y > (*top).y) top = &points[i];
    }
    //endregion

    // Paths
    poly_helper::Node *left_top{nullptr}, *top_right{nullptr}, *right_bottom{nullptr}, *bottom_left{nullptr};

    //region Form path from left to top point
    left_top = poly_helper::form_path(_points);
    //endregion

    //region To form path from right to bottom we mirror each point against the center of coordinate system
    for (auto &p : _points) p *= -1;
    // Rearrange sort
    for (size_t i{0}; i < points.size() / 2; i++) std::swap(_points[i], _points[points.size() - i - 1]);
    // Get path
    right_bottom = poly_helper::form_path(_points);
    // Transform back
    for (auto n{right_bottom}; n != nullptr; n = n->next) n->point *= -1;
    //endregion

    //region To form path from top to right we now rotate 90deg clockwise
    for (auto &p : _points) p = Point{p.y, -p.x};
    // Rearrange sort
    sort();
    if (top != right) {
        // Get path
        top_right = poly_helper::form_path(_points);
        // Transform back
        for (auto n{top_right}; n != nullptr; n = n->next) n->point = Point{n->point.y, -n->point.x};
    }
    //endregion

    //region To form path from bottom to left we mirror each point against the center of coordinate system
    for (auto &p : _points) p *= -1;
    // Rearrange sort
    if (bottom != left) {
        for (size_t i{0}; i < points.size() / 2; i++) std::swap(_points[i], _points[points.size() - i - 1]);
        // Get path
        bottom_left = poly_helper::form_path(_points);
        // Transform back
        for (auto n{bottom_left}; n != nullptr; n = n->next) n->point = Point{-n->point.y, n->point.x};
    }
    //endregion

    //region Connect the paths
    for (auto n{left_top}; n && n->next; n = n->next) this->points.push_back(n->point);
    for (auto n{top_right}; n && n->next; n = n->next) this->points.push_back(n->point);
    for (auto n{right_bottom}; n && n->next; n = n->next) this->points.push_back(n->point);
    for (auto n{bottom_left}; n && n->next; n = n->next) this->points.push_back(n->point);
    //endregion

    //region Free resources
    if (left_top) left_top->free();
    if (top_right) top_right->free();
    if (right_bottom) right_bottom->free();
    if (bottom_left) bottom_left->free();
    //endregion

#undef sort
}

/// Determine if the point lies inside the hull
bool Polygon::contains(const Point &point) const {
    // To check if the point lies inside the hull we check for each segment orientation
    // Each segment consists of hull's line and the point
    // If the point lies on the 'left' side of the line for every segment, then it lies inside the hull

    // Go through all lines
    for (size_t i{0}; i < points.size() - 1; i++)
        // Check if vector to the point is "left" from the vector to the next segment
    {
        float angle = ((points[i + 1] - points[i]) | (point - points[i]));
        if (angle <= 0) return false;
    }

#ifdef VERBOSE
    OUT << "Polygon contains " << point << ENDL;
#endif
    return true;
}

/// Determine if hulls intersect
bool Polygon::intersects(const Polygon &other) const {
    INFO("Checking polygons intersections");

    // The task is to go through all points of the first polygon
    // and see if the second polygon contains any

    // The last point is also the first, so we can skip it
    for (size_t i{0}; i < points.size() - 1; i++)
        if (other.contains(points[i])) return true;


    // If no point is contained there are also cases where polygons might
    // "pass through" each other (e.g. cross from two rectangles)

    // We need to check if any two line segments intersect
    for (size_t i{0}; i < points.size() - 1; i++)
        for (size_t j{0}; j < other.points.size() - 1; j++)
            if (poly_helper::segments_intersect(points[i], points[i + 1], other.points[j], other.points[j + 1])) {
#ifdef VERBOSE
                OUT << "Segments intersect" << ENDL;
#endif
                return true;
            }

    return false;
}