#include <util/math.h>
#include <map>
#include <set>
#include <util/Polynomial.h>
#include "util/code_organizers.h"
#include "logic.h"
#include "random"
#include "Polygon.h"
#include <list>

#define arg_is(argument) current_arg < argc && strcmp(argv[current_arg], "--" #argument) == 0

struct p2d {
    float x{0}, y{0};

    friend std::ostream &operator<<(std::ostream &os, const p2d &p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }

    p2d() {
        OUT << "Empty" << ENDL;
    };

    p2d(float x, float y) : x{x}, y{y} {
        OUT << "Normal" << ENDL;
    }

    p2d(const p2d &other) : x{other.x}, y{other.y} {
        OUT << "Copy" << ENDL;
    }

    p2d(p2d &&other) noexcept : x{other.x}, y{other.y} {
        OUT << "Move" << ENDL;
    }

    p2d &operator=(const p2d &other) {
        OUT << "Copy assignment" << ENDL;
        x = other.x;
        y = other.y;
        return *this;
    }
    p2d &operator=(p2d &&other) noexcept {
        OUT << "Move" << ENDL;
        std::swap(x, other.x);
        std::swap(y, other.y);
        return *this;
    }

    bool operator!=(const p2d &other) const {
        return x != other.x || y != other.y;
    }
    bool operator==(const p2d &other) const {
        return !(*this != other);
    }

};

template<typename T>
void divide(Polynomial<T> dividend, Polynomial<T> divisor) {

    OUT << ENDL << "Dividing " << dividend << "\tby " << divisor << ENDL;

    auto xx{(dividend / divisor)};
    OUT << "Result: " << xx.result;
    OUT << "Reminder: " << xx.remainder;
}

template<typename T>
void roots(Polynomial<T> polynomial) {
    OUT << "Polynomial " << polynomial;
    OUT << "has " << polynomial.roots() << " roots" << ENDL;
}

int main() {
//    Polynomial<p2d> p1{{1, 2},
//                       {3, 4},
//                       {5, 6}};
//    Polynomial<p2d> p2{};
//
//    p1.print();
//    p2.print();
//
//    Polynomial<float> polynomial{1, 2, 3, 4.1f / 3.f};
//
//    OUT << "Polynomial " << polynomial;
//    OUT << "Minus Polynomial " << -polynomial;
//
//    Polynomial<float> polynomial2{1, -3, 3, -4.1f * 2.f / 6.f};
//
//    int x{};
//
//    OUT << "X = " << x << ENDL;
//
//    (-((polynomial + polynomial2) * 4) / 2).print();
//    Polynomial<> zero{};
//    zero.print();
//
//
//    divide(Polynomial<>{4, 2, 8}, Polynomial<>{1, 2, 3});
//    divide(Polynomial<>{1, 2, 3}, Polynomial<>{1, 2, 3});
//    divide(Polynomial<>{1, 2, 3}, Polynomial<>{1, 2, 3, 4});
//    divide(Polynomial<>{1}, Polynomial<>{1, 2, 3, 4});
//    divide(Polynomial<>{0, 0, 0, 0, 0, 6}, Polynomial<>{1, 2, 3, 4});
//    try {
//        divide(Polynomial<>{0, 0, 0, 0, 0, 6}, Polynomial<>{0});
//    } catch (std::runtime_error &) {
//        OUT << "Division by zero throws" << ENDL;
//    }
//    divide(Polynomial<>{0, 0, 3, 0, 0}, Polynomial<>{1, 2, 3, 4});
//    divide(Polynomial<>{5, 0, 3, 4}, Polynomial<>{1, 2, 3, 4});
//    divide(Polynomial<>{5, 8, 3, 4}, Polynomial<>{1, 2, 3, 4});
//
//    Polynomial<> p{1, 2, 3, 4, 5};
//
//    p.print();
//    OUT << "1: " << p.derivative();
//    OUT << "2: " << p.derivative().derivative();
//    OUT << "3: " << p.derivative(3);
//
//    (Polynomial<>{0}).derivative().print();
//    (Polynomial<>{1}).derivative().print();
//    (Polynomial<>{1, 4}).derivative().print();
//    (Polynomial<>{1, 4, 6}).derivative().print();
//    (Polynomial<>{1, 4, 6}).derivative(12).print();
//    (Polynomial<>{1, 4, 6}).derivative(0).print();
//    (Polynomial<>{1, 4, 6}).derivative().derivative().print();
//
//    OUT << Polynomial<>{}(5) << ENDL;
//    OUT << Polynomial<>{1}(5) << ENDL;
//    OUT << Polynomial<>{1, 2}(5) << ENDL;
//    OUT << Polynomial<>{1, 0, 5}(5.5) << ENDL;
//    OUT << Polynomial<>{0, 0, 5, 0, 0, 8, 0}(9) << ENDL;
//
//    OUT << Polynomial<>{}.integrate();
//    OUT << Polynomial<>{1}.integrate();
//    OUT << Polynomial<>{2}.integrate();
//    OUT << Polynomial<>{1, 2}.integrate();
//    OUT << Polynomial<>{4, 4, 8}.integrate();
//
//    roots(Polynomial<>{4, 4, 8});
//
//    OUT << math::sqrt(4) << ENDL;
//    OUT << math::sqrt(5.0) << ENDL;
//
//    printf("%.10lf\n", math::sqrt<double>(5.0));
//    printf("%.10f\n", math::sqrt<float>(5.0));

//    constexpr float precision = math::precision<float>();
//    constexpr double precision2 = math::precision<double>();

//    OUT << Vector{3, 5} * Vector{2, 3} << ENDL;
//    OUT << Vector{3, 5}.length() << ENDL;
    OUT << math::degrees(Vector{0, 1} | (Vector{0, 1})) << ENDL;
    OUT << math::degrees(Vector{0, 1} | (Vector{1, 1})) << ENDL;
    OUT << math::degrees(Vector{0, 1} | (Vector{-1, 1})) << ENDL;
    OUT << math::degrees(Vector{0, 1} | (Vector{-1, -1})) << ENDL;
//    OUT << (Vector{0, 1}.length()) << ENDL;
//    OUT << (Vector{0, 1} * (Vector{0, 1})) << ENDL;
    OUT << "Rad " << (Vector{0, 1} | (Vector{0, 1})) << ENDL;
    OUT << "Deg " << math::degrees(Vector{0, 6} | (Vector{0, 1})) << ENDL;

//    Point points[3];

//    OUT << arr_len(points) << ENDL;

    Polygon hull{Point{1, 2}, Point{3, 4}, Point{4, 5}, Point{2, 8}, Point{3, 14}, Point{7, 5}, Point{-4, 7}, Point{4, 15},
              Point{-4, -15}, Point{5, 13}, Point{5, 0}, Point{6, 9}};

    OUT << hull << ENDL;

    OUT << (hull.contains(Point{0, 0}) ? "contains" : "does not contain") << ENDL;

    Polygon h{Point{1, 3}, Point{4, 2}, Point{1, 4}, Point{3, 7}};

    OUT << h << ENDL;
    OUT << (h.contains(Point{0, 0}) ? "contains" : "does not contain") << ENDL;

    OUT << "Degrees " << math::degrees((Point{4, 4} - Point{2, 2}) | (Point{4, 2} - Point{2, 2})) << ENDL;
//    OUT <<

//    std::list<int> ints{};
//    ints.emplace_back(1);
//    ints.emplace_back(2);
//    ints.emplace_back(3);
//    ints.emplace_back(4);
//    ints.emplace_front(5);
//
//    for (auto x: ints) {
//        OUT << x << " ";
//    }
//    OUT << ENDL;
//
//    OUT << *----------------------------------------------------------------ints.begin() << " ";

//    OUT << math::degrees(Vector{0, 1} | Vector{-1, -1}) << ENDL;
//    OUT << math::degrees(Vector{0, 1} | Vector{1, -1}) << ENDL;

    OUT << math::radians(-180.l) << ENDL;

    OUT << math::modulo<double>(-3.7159, -math::pi<double>()) << ENDL;
    OUT << "this " << ((Point{4, 15} - Point{3, 14}) | (Point{0} - Point{3, 14})) << ENDL;
    OUT << "this " << math::modulo((Point{4, 15} - Point{3, 14}) | (Point{0} - Point{3, 14}), math::pi<float>()) << ENDL;

    Polygon polygon1{ Point{0,0}, Point{1,0}, Point{1,1} },
            polygon2{ Point{0.5,0}, Point{.75,1.5}, Point{0.5,1.5} };
//
//    OUT << "BEGIN" << ENDL;
//    OUT << polygon2.contains({0, 0}) << ENDL;
//    OUT << polygon2.contains({1, 1}) << ENDL;
//    OUT << polygon2.contains({1, 0}) << ENDL;
//    OUT << "END" << ENDL;

    OUT << ((polygon1.intersects(polygon2)) ? "intersect" : "don't intersect") << ENDL;

    OUT << math::degrees((Vector{1,0}) | (Vector{})) << ENDL;

    return 0;

    Params params{};

//    if (argc > 1) {
//        int current_arg{1};
//
//        if (arg_is(help)) {
//            Log("Displaying info for Bezier");
//
//            return 0;
//        }
//
//        if (arg_is(test)) {
//            Log("Test mode");
//
//            return run_test();
//        }
//
//        if (arg_is(count)) {
//            current_arg++;
//            params.curves_count = static_cast<size_t>(std::strtol(argv[current_arg++], nullptr, 10));
//        }
//
//        if (arg_is(min)) {
//            current_arg++;
//            params.points_min = static_cast<size_t>(std::strtol(argv[current_arg++], nullptr, 10));
//        }
//
//        if (arg_is(max)) {
//            current_arg++;
//            params.points_max = static_cast<size_t>(std::strtol(argv[current_arg++], nullptr, 10));
//        }
//
//        require(current_arg == argc, "Unknown arguments format. Please see -help for info.");

    generate(params);
//    }

// 1. -help - one arg
// 2. get points from csv file

    return 0;
}