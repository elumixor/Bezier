//
// Created by Vladyslav Yazykov on 30/12/2018.
//

#include "Hull.h"

struct Path {
    Path *next;
    const Point *point;
};

struct Line {
    const Point &p1, &p2;
    bool point_above(const Point &p) const {

    }
};

Hull::~Hull() {
    delete[] points;
}
Hull::Hull(const Point *points, size_t length) {
    // Find the bounding box
    OUT << length << ENDL;

    const Point *left{points}, *right{left}, *top{right}, *bottom{top};
    for (size_t i{0}; i < length; i++) {
        // Left
        if (points[i].x < left->x) left = points + i;
            // Right
        else if (points[i].x > right->x) right = points + i;
        // Top
        if (points[i].y > top->y) top = points + i;
            // Bottom
        else if (points[i].y < bottom->y) bottom = points + i;
    }

    Path *left_top = new Path{nullptr, left};


    // Path from left to top
    if (left != top) {

        // Form line through left and top points
        Line line{*left, *top};

        // Iterate through points
        for (size_t i{0}; i < length; i++) {

            // Exclude beginning point
            if (&points[i] != left
                // Lies "above" the line from left to top
                && line.point_above(points[i])) {

            }
        }
    }

    // Find line
    // For each that higher
    // Get angle

    OUT << "Left: " << *left << ". Right " << *right << ENDL;
    OUT << "Top: " << *top << ". Bottom " << *bottom << ENDL;
}
Hull::Hull(std::initializer_list<Point> points) : Hull(points.begin(), points.size()) {}
Hull::Hull(const Hull &other) : count{other.count}, points{new Point[count]} {
    for (size_t i{0}; i < count; i++) points[i] = other.points[i];
}
