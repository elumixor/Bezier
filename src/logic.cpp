//
// Created by Vladyslav Yazykov on 26/12/2018.
//

#include <util/code_organizers.h>
#include "logic.h"

int run_test() {
    return 0;
}

int generate(const Params &params) {
    OUT << params.curves_count << " "
        << params.points_min << " "
        << params.points_max << ENDL;

    std::forward_list<Bezier<Point>> curves{};

    for (size_t i{0}; i < params.curves_count; ++i) {
        // Number of control points
        auto point_count = (size_t) math::random_int(static_cast<int>(params.points_min),
                                                     static_cast<int>(params.points_max));

        Point points[point_count];

        // Generate points
        for (Point &p: points)
            p = {math::random_float(), math::random_float()};

        curves.push_front(Bezier<Point>(point_count, points));

#ifdef VERBOSE
        OUT << ENDL;
#endif
    }

    unsigned _intersections{0};

    for (const Bezier<Point> &c1 : curves)
        for (const Bezier<Point> &c2: curves)  // also check for self-intersection
            _intersections += intersections(c1, c2);

    return _intersections;

}