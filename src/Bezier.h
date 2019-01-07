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
#include "cassert"

/// Bezier curve class
template<typename Point>
class Bezier final {
    struct pair {
        const Bezier first;
        const Bezier second;
    };

public:
    //region Fields

    size_t n;
    Point *control_points;

#ifdef COMPUTE_BEZIER
    Point *C;
    Point integrated{};
#endif
    //endregion

    //region Constructors/Assignments

    /// Creates new bezier curve object
    //region Basic constructor
#ifdef COMPUTE_BEZIER
    Bezier(size_t count, const Point *points) : n{count - 1}, control_points{new Point[count]}, C{new Point[count]} {
#else
    Bezier(const Point *points, size_t count) : n{count - 1}, control_points{new Point[count]} {
#endif
        assert(n > 0 && "Bezier curve can be created from 2 and more points");

        std::copy(points, points + count, control_points);

        //region VERBOSE
#ifdef VERBOSE
        OUT << "Creating bezier curve from points: ";
        for (size_t i{0}; i <= n; i++) OUT << points[i] << ", ";
        OUT << ENDL;
#endif
        //endregion

        //region COMPUTE_BEZIER
        // Initially computed bezier coefficients to express the curve in polynomial form,
        // but unnecessary for solving intersections problem
#ifdef COMPUTE_BEZIER
        // Using the polynomial formula
        for (size_t j{0}; j <= n; j++) {
            auto f = math::range_product(n - j + 1, n);

            Point sum{};
            for (size_t i{0}; i <= j; i++)
                sum = sum + (points[i] * (float) math::sign(i + j)
                             * (1.f / (float) (math::factorial(i) * math::factorial(j - i))));

            C[j] = sum * (float) f;

            // Integrating bezier curve from 0 to 1 can be useful if we have to deal with the offset
            // e.g. if we want to use bezier curve to describe animation easing and still want it to take constant time
            integrated = integrated + C[j] * (1.f / (float) (j + 1));
        }

#ifdef VERBOSE
        OUT << "Bezier coefficients are: ";
        for (size_t i{0}; i <= n; ++i) OUT << C[i] << ", ";
        OUT << ENDL << "Integrated  = " << integrated << ENDL;
#endif
#endif
        //endregion
    }
    //endregion

#ifdef COMPUTE_BEZIER
    Bezier(const Bezier &other) : n{other.n}, control_points{new Point[n + 1]}, C{new Point[n + 1]}, integrated{other.integrated} {
        COPY(C, other.C, n + 1);
        COPY(control_points, other.control_points, n + 1);
    }

    Bezier(Bezier &&other) noexcept : n{other.n}, control_points{other.control_points},
                                    C{other.C}, integrated{other.integrated} {
        other.C = nullptr;
        other.control_points = nullptr;
    };
    Bezier &operator=(const Bezier &o) {
        n = o.n;
        integrated = o.integrated;

        delete[] C;
        delete[] control_points;

        C = new Point[n + 1];
        control_points = new Point[n + 1];

        COPY(C, o.C, n + 1);
        COPY(control_points, o.control_points, n + 1);

        return *this;
    };
    Bezier &operator=(Bezier &&o) noexcept {
        n = o.n;
        integrated = o.integrated;
        C = o.C;
        control_points = o.control_points;
        o.C = nullptr;
        o.control_points = nullptr;
        return *this;
    };
#else
    Bezier(const Bezier &other) : n{other.n}, control_points{new Point[n + 1]} {
        COPY(control_points, other.control_points, n + 1);
    }
    Bezier(Bezier &&other) noexcept : n{other.n}, control_points{other.control_points} {
        other.control_points = nullptr;
    };
    Bezier &operator=(const Bezier &o) {
        n = o.n;

        delete[] control_points;

        control_points = new Point[n + 1];

        COPY(control_points, o.control_points, n + 1);

        return *this;
    };
    Bezier &operator=(Bezier &&o) noexcept {
        n = o.n;
        control_points = o.control_points;
        o.C = nullptr;
        o.control_points = nullptr;
        return *this;
    };
#endif
    //endregion

    Bezier(std::initializer_list<Point> points) : Bezier(points.begin(), points.size()) {}

    ~Bezier() {
#ifdef COMPUTE_BEZIER
        delete[] C;
#endif
        delete[] control_points;
    }

    //region Calculate bezier at point (IF COMPUTE)
#ifdef COMPUTE_BEZIER
    /// Calculate parametric curve value at point
    Point operator()(float t) const {
            Point r{};
            for (size_t j{0}; j <= n; j++)
                r += math::pow(t, (int) j) * C[j];

            return r;
        }
#endif
    //endregion

    /// Subdivide the bezier curve
    pair subdivide(float at = .5f) const {
        size_t levels{n + 1};

        Point *points[levels];
        points[0] = control_points;

        for (size_t i{1}; i < levels; i++) {
            points[i] = new Point[levels - i];
            for (size_t j{0}; j < levels - i; j++)
                points[i][j] = points[i - 1][j] * at + points[i - 1][j + 1] * (1 - at);
        }

        Point points_a[levels];
        Point points_b[levels];

        for (size_t i{0}; i < levels; i++)
            points_a[i] = points[i][0];

        for (size_t i{0}; i < levels; i++)
            points_b[i] = points[i][levels - i - 1];

        // Cleanup
        for (size_t i{1}; i < levels; i++)
            delete[] points[i];

        return {{points_a, levels},
                {points_b, levels}};
    }
};

/// Calculate the number of intersections with the other curve
unsigned intersections(const Bezier<Point> &a, const Bezier<Point> &b, size_t recursion_step = 0);

#endif // BEZIER_BEZIER_H
