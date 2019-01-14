#include <util/math.h>
#include <map>
#include <set>
#include "util/code_organizers.h"
#include "random"
#include "Polygon.h"
#include <list>
#include "logic.h"

/// timer
template<typename TimePoint>
std::chrono::milliseconds to_ms(TimePoint tp) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(tp);
}

/// Get point from input
static Point get_point() {
    char s;
    float v1, v2;
    std::cin >> s >> v1 >> s >> v2 >> s;
    if (std::cin.fail()) throw std::invalid_argument("Invalid syntax. Please see --help for more details.");

    return {v1, v2};
}

/// Get curve from input
Bezier<Point> get_curve() {
    OUT << "Points count: ";
    int points_count;
    std::cin >> points_count;

    OUT << points_count << ENDL;

    if (std::cin.fail() || points_count < 2)
        throw std::invalid_argument("Bezier curve should have at least 2 control points.");

    Point points[points_count];

    for (size_t i{0}; i < static_cast<size_t>(points_count); i++) {
        points[i] = get_point();
    }

    return Bezier<Point>(points, static_cast<size_t>(points_count));
}

struct curve_pair {
    const Bezier<Point> &c1;
    const Bezier<Point> &c2;
};

unsigned intrs{0};

void *get_curves_intersections(void *arg) {
    auto c_pair = static_cast<curve_pair *>(arg);
    intrs += curve_intersections(c_pair->c1, c_pair->c2);
    pthread_exit(nullptr);
}

/// get curves intersections
void get_intersections(const std::forward_list<Bezier<Point>> &curves) {
    unsigned intersections{0};

    //region Sequential
    auto start = std::chrono::high_resolution_clock::now();

    for (auto c1{curves.begin()}; c1 != curves.end(); c1++) {
        for (auto c2{c1}; ++c2 != curves.end();) { // Omit self intersections
            intersections += curve_intersections(*c1, *c2);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    OUT << "Sequential computation took " << to_ms(end - start).count() << " ms." << ENDL;
    OUT << "Intersections: " << intersections << ENDL;
    //endregion

    //region Parallel

    auto curves_count{std::distance(std::begin(curves), std::end(curves))};
    auto count{(curves_count * (curves_count - 1)) / 2};

    pthread_t threads[count];

    start = std::chrono::high_resolution_clock::now();
    long i{0};
    for (auto c1{curves.begin()}; c1 != curves.end(); c1++) {
        for (auto c2{c1}; ++c2 != curves.end();) { // Omit self intersections
            curve_pair c{*c1, *c2};
            pthread_create(&threads[i++], nullptr, get_curves_intersections, (void *) &c);
        }
    }

    for (i = 0; i < count; i++) {
        pthread_join(threads[i++], nullptr);
    }

    end = std::chrono::high_resolution_clock::now();
    OUT << "Parallel computation took " << to_ms(end - start).count() << " ms." << ENDL;
    OUT << "Intersections: " << intrs << ENDL;

    //endregion
}
std::forward_list<Bezier<Point>> random_curves(size_t count) {
#ifdef VERBOSE
    OUT << "Generating " << count << " bezier curves" << ENDL;
#endif

    OUT << "Count: " << count << ENDL;

    std::forward_list<Bezier<Point>> curves{};

    for (size_t i{0}; i < count; ++i) {
        // Number of control points
        auto point_count{static_cast<size_t>(math::random_int(2, MAX_CURVE_DEGREE + 1))};

        Point points[point_count];

        // Generate points
        for (Point &p: points)
            p = {math::random_float(-10, 10), math::random_float(-10, 10)};

        curves.push_front(Bezier<Point>(points, point_count));

#ifdef VERBOSE
        OUT << "Generated bezier curve:\n" << curves.front() << ENDL;
        NEWL;
#endif
    }

    return curves;
}

unsigned curve_intersections(const Bezier<Point> &a, const Bezier<Point> &b, size_t recursion_step) {
#ifdef VERBOSE
    OUT << "Checking intersections. Recursion step: " << recursion_step << ENDL;
#endif

    // To calculate intersections we try to figure out if the convex hulls of the curves intersect.
    // If hulls do not intersect, the algorithm is stopped.
    //
    // Otherwise we subdivide the curves using De Casteljau's algorithm
    // https://en.wikipedia.org/wiki/De_Casteljau%27s_algorithm
    // for as much as precision/recursion depth allows.

    // Convex hull of a curve is given by its control points

    // If polygons do not intersect, then curves also do not intersect
    if (!Polygon(a).intersects(Polygon(b))) return 0;

    // Within this precision curves do intersect
    if (recursion_step == RECURSION_LIMIT) return 1;

    // Otherwise subdivide curves in half
    auto a_subdivided{a.subdivide()};
    auto b_subdivided{b.subdivide()};

    // And count their intersections
    return curve_intersections(a_subdivided.first, b_subdivided.first, recursion_step + 1)
           + curve_intersections(a_subdivided.first, b_subdivided.second, recursion_step + 1)
           + curve_intersections(a_subdivided.second, b_subdivided.first, recursion_step + 1)
           + curve_intersections(a_subdivided.second, b_subdivided.second, recursion_step + 1);
}
