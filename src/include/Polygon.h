//
// Created by Vladyslav Yazykov on 30/12/2018.
//

#ifndef BEZIER_HULL_H
#define BEZIER_HULL_H

#include <ostream>
#include "BazicPoint.h"
#include "util/code_organizers.h"
#include "Bezier.h"

class Polygon {
    size_t count{0};
    Point *points{nullptr};
public:
    Polygon(const Point *points, size_t length);
    Polygon(const Polygon &other);
    Polygon(std::initializer_list<Point> points);
    Polygon(const Bezier<Point> &curve);

    virtual ~Polygon();

    friend std::ostream &operator<<(std::ostream &os, const Polygon &hull);

    /// Determine if the point lies inside the hull
    bool contains(const Point &point) const;

    /// Determine if hulls intersect
    bool intersects(const Polygon& other) const;
};

namespace _poly_helper_ {
    /// Helper struct for iteration
    struct Node {
        Node *next{nullptr}, *previous;
        Point point;

        /// Add node
        explicit Node(Point point, Node *previous = nullptr) : previous{previous}, point{std::move(point)} {}

        Node() = delete;
        Node(const Node &) = delete;
        Node(Node &&) = delete;
        Node &operator=(const Node &) = delete;
        Node &operator=(Node &&) = delete;

        /// Remove node, free memory
        void free();
    };

    enum Orientation {
        Clockwise, Counterclockwise, Collinear
    };

    Orientation orientation(const Point &a, const Point &b, const Point &c);

    /// Compute the angle between current vector and previous
    constexpr float compute_angle(const Point &current, const Point &last, const Point &previous) {
        return (last - previous) | (current - last);
    }

    /// Determine if two line segments intersect
    bool segments_intersect(const Point &p1, const Point &q1, const Point &p2, const Point &q2);

    /// Form the path form left to top
    Node *form_path(const Point *points, size_t length);
}


#endif //BEZIER_HULL_H
