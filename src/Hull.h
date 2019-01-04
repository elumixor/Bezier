//
// Created by Vladyslav Yazykov on 30/12/2018.
//

#ifndef BEZIER_HULL_H
#define BEZIER_HULL_H

#include "BasicPoint.h"
#include "util/code_organizers.h"

class Hull {
    size_t count{0};
    Point *points{nullptr};
public:
    Hull(const Point *points, size_t length);
    Hull(const Hull &other);
    Hull(std::initializer_list<Point> points);
    virtual ~Hull();
};


#endif //BEZIER_HULL_H
