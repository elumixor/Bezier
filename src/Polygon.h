//
// Created by Vladyslav Yazykov on 30/12/2018.
//

#ifndef BEZIER_HULL_H
#define BEZIER_HULL_H

#include <ostream>
#include "BasicPoint.h"
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


#endif //BEZIER_HULL_H
