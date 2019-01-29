#include <util/math.h>
#include <map>
#include <set>
#include "Polygon.h"
#include <list>
#include "application_logic.h"

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

    std::vector<Point> points;

    for (size_t i{0}; i < static_cast<size_t>(points_count); i++) {
        points[i] = get_point();
    }

    return Polygon(points);
}

unsigned intrs{0};

std::vector<Polygon> data;
size_t total{0};

void *get_polygons_intersections(void *) {
    static size_t called{0};
    auto start = called++;
    for (size_t i{start + 1}; i < total; i++)
        intrs += (data[start]).intersects(data[i]) ? 1 : 0;

    pthread_exit(nullptr);
}

/// Get polygons' intersections
void get_intersections(const std::vector<Polygon> &poly) {
    total = std::distance(std::begin(poly), std::end(poly));

    OUT << "Total: " << total << ENDL;
    std::vector<Polygon> polygons(poly);
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
    std::vector<pthread_t> threads;
    threads.reserve(total);

    start = std::chrono::high_resolution_clock::now();

    for (size_t i{0}; i < total; i++)
        pthread_create(&threads[i], nullptr, get_polygons_intersections, (void *) &i);

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

std::vector<Polygon> random_polygons(size_t polygons_count, size_t max_points_in_polygon) {
#ifdef VERBOSE
    OUT << "Generating " << count << " random polygons" << ENDL;
#endif

    std::vector<Polygon> polygons;
    polygons.reserve(polygons_count);

    for (size_t i{0}; i < polygons_count; ++i) {

        // Generate random number of control points
        auto points_in_polygon{static_cast<size_t>(math::random_int(2, static_cast<int>(max_points_in_polygon)))};

        std::vector<Point> points;
        points.reserve(points_in_polygon);

        // Generate random points
        for (size_t j{0}; j < points_in_polygon; j++)
            points.emplace_back(Point(math::random_float(-10, 10), math::random_float(-10, 10)));

        // Create a polygon and add it to a vector
        polygons.emplace_back(Polygon(points));

#ifdef VERBOSE
        OUT << "Generated polygon:\n" << polygons.front() << ENDL;
        NEWL;
#endif
    }

    return polygons;
}