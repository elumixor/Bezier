//
// Created by Vladyslav Yazykov on 2019-01-29.
//

#include "Polygon.h"

void poly_helper::Node::free() {
    if (previous) previous->next = nullptr;
    if (next) next->free();
    next = nullptr;
    delete (this);
}

bool poly_helper::segments_intersect(const Point &p1, const Point &q1, const Point &p2, const Point &q2) {
    // Inspired by algorithm from https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect
    // We need to check for orientations: A(p1, q1, p2), B(p1, q1, q2), C(p2, q2, p1), D(p2, q2, q1)

    Orientation
            A{orientation(p1, q1, p2)},
            B{orientation(p1, q1, q2)},
            C{orientation(p2, q2, p1)},
            D{orientation(p2, q2, q1)};

    if (A == Collinear || B == Collinear || C == Collinear || D == Collinear) return false;

#ifdef VERBOSE
    if (A != B && C != D) {
        OUT << "Segments (" << p1 << ", " << q1 << ") and (" << p2 << ", " << q2 << ") intersect." << ENDL;
        return true;
    } else return false;
#else
    return A != B && C != D;
#endif
}

poly_helper::Node *poly_helper::form_path(const std::vector<Point> &points) {
    // To form the path we compute the angle between current vector and previous vector
    // If the angle is less than zero, delete previous point and count the angle again
    // Repeat until the angle is greater or equal to zero

    Node *start{new Node(points[0])}, *last{start};

    for (size_t i{1}; i < points.size(); i++) {
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

poly_helper::Orientation poly_helper::orientation(const Point &a, const Point &b, const Point &c) {
    float angle{(b - a) | (c - b)};
    if (math::equal(angle, math::pi()) || math::equal(angle, -math::pi()) || math::equal(angle, 0.f)) return Collinear;
    return angle > 0 ? Clockwise : angle < 0 ? Counterclockwise : Collinear;
}
