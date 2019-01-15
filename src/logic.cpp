#include <util/math.h>
#include <map>
#include <set>
#include "util/code_organizers.h"
#include "random"
#include "Polygon.h"
#include <list>
#include "logic.h"

/// Timer
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
Polygon get_polygon() {
    OUT << "Points count: ";
    int points_count;
    std::cin >> points_count;

    if (std::cin.fail() || points_count < 2)
        throw std::invalid_argument("Polygon should have at least 2 control points.");

    Point points[points_count];

    for (size_t i{0}; i < static_cast<size_t>(points_count); i++) {
        points[i] = get_point();
    }

    return Polygon(points, static_cast<size_t>(points_count));
}

struct polygon_pair {
    size_t at;
    size_t total;
    const Polygon *polygons;
};

unsigned intrs{0};

Polygon *data{nullptr};
size_t total{0};

void *get_polygons_intersections(void *) {
    static size_t called{0};
    auto start = called++;
    for (size_t i{start + 1}; i < total; i++)
        intrs += (data[start]).intersects(data[i]) ? 1 : 0;

    pthread_exit(nullptr);
}

/// Get polygons' intersections
void get_intersections(const std::forward_list<Polygon> &poly) {
    total = std::distance(std::begin(poly), std::end(poly));

    OUT << "Total: " << total << ENDL;
    Polygon polygons[total];
    {
        size_t i{0};
        for (const auto &p: poly)
            polygons[i++] = p;

    }

    data = polygons;

    unsigned intersections{0};

    //region Sequential
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i{0}; i < total; ++i) {
        for (size_t j{i + 1}; j < total; ++j) {
            intersections += data[i].intersects(data[j]) ? 1 : 0;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    OUT << "Sequential computation took " << to_ms(end - start).count() << " ms." << ENDL;
    OUT << "Intersections: " << intersections << ENDL;

    //region Parallel
    pthread_t threads[total];

    start = std::chrono::high_resolution_clock::now();

    for (size_t i{0}; i < total; i++) {
        pthread_create(&threads[i], nullptr, get_polygons_intersections, (void *) &i);
    }

    for (size_t i = 0; i < total; i++)
        pthread_join(threads[i++], nullptr);

    end = std::chrono::high_resolution_clock::now();
    OUT << "Parallel computation took " << to_ms(end - start).count() << " ms." << ENDL;
    OUT << "Intersections: " << intrs << ENDL;

    OUT << "Numeric instability: " <<
        ((math::max(intrs, intersections) - math::min(intrs, intersections))
         / (double) math::max(intrs, intersections)) << ENDL;
    //endregion
}

std::forward_list<Polygon> random_polygons(size_t count) {
#ifdef VERBOSE
    OUT << "Generating " << count << " random polygons" << ENDL;
#endif

    std::forward_list<Polygon> curves{};

    for (size_t i{0}; i < count; ++i) {

        // Number of control points
        auto point_count{static_cast<size_t>(math::random_int(2, MAX_CURVE_DEGREE + 1))};

        Point points[point_count];

        // Generate points
        for (Point &p : points)
            p = {math::random_float(-10, 10), math::random_float(-10, 10)};
        curves.push_front(Polygon(points, point_count));

#ifdef VERBOSE
        OUT << "Generated bezier curve:\n" << curves.front() << ENDL;
        NEWL;
#endif
    }

    return curves;
}