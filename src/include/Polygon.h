//
// Created by Vladyslav Yazykov on 30/12/2018.
//

#ifndef BEZIER_HULL_H
#define BEZIER_HULL_H

#include <ostream>
#include "BazicPoint.h"
#include "util/code_organizers.h"

/// Two-dimensional polygon with convex hull
class Polygon {
    /// Points that form convex hull of the polygon
    std::vector<Point> points{};
public:
    /// Main constructor
    explicit Polygon(std::vector<Point> points);

    /// Empty polygon is forbidden
    Polygon() = delete;

    /// Constructor from points as initializer list
    Polygon(std::initializer_list<Point> points) : Polygon(std::vector<Point>{points}) {};

    /// Output to a stream
    inline friend std::ostream &operator<<(std::ostream &os, const Polygon &polygon) {
        for (const Point &p: polygon.points) os << p << " ";
        return os;
    }

    /// Determine if the point lies inside the hull
    bool contains(const Point &point) const;

    /// Determine if hulls intersect
    bool intersects(const Polygon &other) const;
};

/// Contains helper algorithms. Created for higher cohesion
namespace poly_helper {

    /// Helper structure for iteration. Used to create a path of vertices
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

    /// Segments orientation
    enum Orientation {
        Clockwise, Counterclockwise, Collinear
    };

    Orientation orientation(const Point &a, const Point &b, const Point &c);

    /// Compute the angle between current vector and previous
    inline float compute_angle(const Point &current, const Point &last, const Point &previous) {
        return (last - previous) | (current - last);
    }

    /// Determine if two line segments intersect
    bool segments_intersect(const Point &p1, const Point &q1, const Point &p2, const Point &q2);

    /// Form the path form left to top
    Node *form_path(const std::vector<Point> &);
}

#endif //BEZIER_HULL_H
