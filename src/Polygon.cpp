//
// Created by Vladyslav Yazykov on 30/12/2018.
//

#include <utility>
#include <string>
#include <map>
#include <sstream>
#include <list>
#include "Polygon.h"

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

    /// Free memory
    void free() {
        if (previous) previous->next = nullptr;
        if (next) next->free();
        next = nullptr;
        delete (this);
    }
};

/// Compute the angle between current vector and previous
static inline constexpr float compute_angle(const Point &current, const Point &last, const Point &previous) {
    return (last - previous) | (current - last);
}

/// Form the path form left to top
static Node *form_path(const Point *const points, size_t length) {
    // To form the path we compute the angle between current vector and previous vector
    // If the angle is less than zero, delete previous point and count the angle again
    // Repeat until the angle is greater or equal to zero

    Node *start{new Node(points[0])}, *last{start};

    for (size_t i{1}; i < length; i++) {
        // Pass if is below the last point
        if (points[i].y < last->point.y) continue;

        float vector_angle{0};
#define angle (vector_angle = last->previous ? compute_angle(points[i], last->point, last->previous->point) : 1)

        // Check the angle if more than one dot are already in path
        if (angle >= 0) {
            last = last->next = new Node(points[i], last);
            continue;
        }

        // While angle is less than zero
        while (vector_angle < 0) {
            // Delete the last point
            auto previous{last->previous};
            delete last;
            last = previous;

            // Compute angles again
            angle;
        }

        // Now add the point
        last = last->next = new Node(points[i], last);
    }

#undef angle

    return start;
}

/// Construct the polygon's convex hull from points
Polygon::Polygon(const Point *points, size_t length) {
    if (length < 3) throw std::invalid_argument("Polygon requires at least three points");

    // Copy array to allow for geometrical transformations
    Point _points[length];
    std::copy(points, points + length, _points);

    // Sort points horizontally, then vertically
#define sort() std::sort(_points, _points + length, [](auto a, auto b) { return a.x < b.x || (a.x == b.x && a.y < b.y); });
    sort();

    //region Get bounding points
    const Point *left{points}, *top{left}, *right{top}, *bottom{right};

    for (size_t i{0}; i < length; i++) {
        if (points[i].x < (*left).x) left = &points[i];
        else if (points[i].x > (*right).x) right = &points[i];
        if (points[i].y < (*bottom).y) bottom = &points[i];
        else if (points[i].y > (*top).y) top = &points[i];
    }
    //endregion

    // Paths
    Node *left_top{nullptr}, *top_right{nullptr}, *right_bottom{nullptr}, *bottom_left{nullptr};

    //region Form path from left to top point
    left_top = form_path(_points, length);
    //endregion

    //region To form path from right to bottom we mirror each point against the center of coordinate system
    for (auto &p : _points) p *= -1;
    // Rearrange sort
    for (size_t i{0}; i < length / 2; i++) std::swap(_points[i], _points[length - i - 1]);
    // Get path
    right_bottom = form_path(_points, length);
    // Transform back
    for (auto n{right_bottom}; n != nullptr; n = n->next) n->point *= -1;
    //endregion

    //region To form path from top to right we now rotate 90deg clockwise
    for (auto &p : _points) p = Point{p.y, -p.x};
    // Rearrange sort
    sort();
    if (top != right) {
        // Get path
        top_right = form_path(_points, length);
        // Transform back
        for (auto n{top_right}; n != nullptr; n = n->next) n->point = Point{n->point.y, -n->point.x};
    }
    //endregion

    //region To form path from bottom to left we mirror each point against the center of coordinate system
    for (auto &p : _points) p *= -1;
    // Rearrange sort
    if (bottom != left) {
        for (size_t i{0}; i < length / 2; i++) std::swap(_points[i], _points[length - i - 1]);
        // Get path
        bottom_left = form_path(_points, length);
        // Transform back
        for (auto n{bottom_left}; n != nullptr; n = n->next) n->point = Point{-n->point.y, n->point.x};
    }
    //endregion

    //region Connect the paths
    for (auto n{left_top}; n && n->next; n = n->next) count++;
    for (auto n{top_right}; n && n->next; n = n->next) count++;
    for (auto n{right_bottom}; n && n->next; n = n->next) count++;
    for (auto n{bottom_left}; n && n->next; n = n->next) count++;

    this->points = new Point[count];
    size_t i = 0;

    for (auto n{left_top}; n && n->next; n = n->next) this->points[i++] = n->point;
    for (auto n{top_right}; n && n->next; n = n->next) this->points[i++] = n->point;
    for (auto n{right_bottom}; n && n->next; n = n->next) this->points[i++] = n->point;
    for (auto n{bottom_left}; n && n->next; n = n->next) this->points[i++] = n->point;
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
    for (size_t i{0}; i < count; i++)
        // Check if vector to the point is "left" from the vector to the next segment
    {
        float angle = ((points[i + 1] - points[i]) | (point - points[i]));
        if (angle <= 0) return false;
    }

    return true;
}

enum Orientation {
    Clockwise, Counterclockwise, Collinear
};

inline static Orientation orientation(const Point &a, const Point &b, const Point &c) {
    float angle{(c - b) | (b - a)};
    return angle > 0 ? Clockwise : angle < 0 ? Counterclockwise : Collinear;
}

/// Determine if two line segments intersect
static bool segments_intersect(const Point &p1, const Point &q1, const Point &p2, const Point &q2) {
    // Inspired by algorithm from https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect
    // We need to check for orientations: A(p1, q1, p2), B(p1, q1, q2), C(p2, q2, p1), D(p2, q2, q1)

    OUT << "Checking segments (" << p1 << ", " << q1 << ") and (" << p2 << ", " << q2 << ")" << ENDL;

    Orientation
            A{orientation(p1, q1, p2)},
            B{orientation(p1, q1, q2)},
            C{orientation(p2, q2, p1)},
            D{orientation(p2, q2, q1)};

    if (A == Collinear || B == Collinear || C == Collinear || D == Collinear) return false;
    return A != B && C != D;
}

/// Determine if hulls intersect
bool Polygon::intersects(const Polygon &other) const {

    // The task is to go through all points of the first polygon
    // and see if the second polygon contains any

    // The last point is also the first, so we can skip it
    for (size_t i{0}; i < count - 1; i++)
        if (other.contains(points[i])) return true;


    // If no point is contained there are also cases where polygons might
    // "pass through" each other (e.g. cross from two rectangles)

    // We need to check if any two line segments intersect
    for (size_t i{0}; i < count - 1; i++)
        for (size_t j{0}; j < other.count - 1; j++)
            if (segments_intersect(points[i], points[i + 1], other.points[j], other.points[j + 1])) {
                OUT << "Segments intersect" << ENDL;
                return true;
            }

    return false;
}

//region Other constructors, output

Polygon::Polygon(std::initializer_list<Point> points) : Polygon(points.begin(), points.size()) {}
Polygon::Polygon(const Polygon &other) : count{other.count}, points{new Point[count]} {
    std::copy(other.points, other.points + count, points);
}
Polygon::~Polygon() {
    delete[] points;
}
std::ostream &operator<<(std::ostream &os, const Polygon &hull) {
    for (size_t i{0}; i < hull.count; i++) os << hull.points[i] << " ";
    return os;
}
Polygon::Polygon(const Bezier<Point> &curve) : Polygon(curve.control_points, curve.n + 1) {}

//endregion
