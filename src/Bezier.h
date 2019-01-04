//
// Created by Vladyslav Yazykov on 17/12/2018.
//

#ifndef BEZIER_BEZIER_H
#define BEZIER_BEZIER_H

#include "util/math.h"
#include "initializer_list"
#include "utility"
#include "iostream"
#include "util/code_organizers.h"
#include "BasicPoint.h"


template<typename Point>
class Bezier final {
public:
    size_t n;
    Point *C;
    Point integrated{};

    /// Creates new bezier curve object
    Bezier(size_t count, const Point *points) : n{count - 1}, C{new Point[count]} {
        //region VERBOSE
#ifdef VERBOSE
        OUT << "Creating bezier curve from points: ";
        for (size_t i{0}; i <= n; i++) OUT << points[i] << ", ";
        OUT << ENDL;
#endif
        //endregion

        // Using the polynomial formula
        for (size_t j{0}; j <= n; j++) {
            auto f = math::range_product(n - j + 1, n);

            Point sum{};
            for (size_t i{0}; i <= j; i++)
                sum = sum + (points[i] * (float) math::sign(i + j)
                             * (1.f / (float) ( math::factorial(i) * math::factorial(j - i))));

            C[j] = sum * (float) f;

            // Integrating bezier curve from 0 to 1 can be useful if we have to deal with the offset
            // e.g. if we want to use bezier curve to describe animation easing and still want it to take constant time
            integrated = integrated + C[j] * (1.f / (float) (j + 1));
        }

        //region VERBOSE
#ifdef VERBOSE
        OUT << "Bezier coefficients are: ";
        for (size_t i{0}; i <= n; ++i) OUT << C[i] << ", ";
        OUT << ENDL << "Integrated  = " << integrated << ENDL;
#endif
        //endregion
    }

    ~Bezier() { delete[] C; }

    /// Calculate parametric curve at point
    Point operator()(float t) const {
        Point r{};
        for (size_t j{0}; j <= n; j++)
            r += math::pow(t, (int) j) * C[j];

        return r;
    }

    Bezier(const Bezier &o) : n{o.n}, integrated{o.integrated}, C{new Point[n + 1]} {
        for (int i = 0; i <= n; ++i)
            C[i] = o.C[i];
    }
    Bezier(Bezier &&o) noexcept : n{o.n}, C{o.C}, integrated{o.integrated} {
        o.C = nullptr;
    };
    Bezier &operator=(const Bezier &o) {
        n = o.n;
        integrated = o.integrated;
        C = new Point[n + 1];
        for (int i = 0; i <= n; ++i)
            C[i] = o.C[i];
        return *this;
    };
    Bezier &operator=(Bezier &&o) noexcept {
        n = o.n;
        integrated = o.integrated;
        C = o.C;
        o.C = nullptr;
        return *this;
    };
};

/// Calculate the number of intersections with the other curve
unsigned intersections(const Bezier<Point> &a, const Bezier<Point> &b);

#endif // BEZIER_BEZIER_H
