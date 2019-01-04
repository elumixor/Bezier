//
// Created by Vladyslav Yazykov on 29/12/2018.
//

#ifndef BEZIER_POINT_H
#define BEZIER_POINT_H

#include "forward_list"
#include "util/code_organizers.h"
#include "util/math.h"

/// Basic 2D point
template<typename T = float>
class BasicPoint {
    static constexpr T ZeroT{};
public:
    /// Coords
    T x{}, y{};

    // Constructors
    inline constexpr BasicPoint() = default;
    inline constexpr explicit BasicPoint(T val) : x{val}, y{val} {}
    inline constexpr BasicPoint(T x, T y) : x{x}, y{y} {}
    inline constexpr BasicPoint(const BasicPoint &other) : x{other.x}, y{other.y} {}
    inline constexpr BasicPoint(BasicPoint &&other) noexcept : x{other.x}, y{other.y} { other.x = other.y = ZeroT; }

    // Assignment
    inline constexpr BasicPoint &operator=(const BasicPoint &other) {
        x = other.x;
        y = other.y;
        return *this;
    }
    inline constexpr BasicPoint &operator=(BasicPoint &&other) noexcept {
        x = std::move(other.x);
        y = std::move(other.y);
        return *this;
    };

    // Arithmetic operations
    inline constexpr BasicPoint operator+(const BasicPoint &other) const { return {x + other.x, y + other.y}; }
    inline constexpr BasicPoint operator-(const BasicPoint &other) const { return {x - other.x, y - other.y}; }
    inline constexpr BasicPoint operator*(const T &scalar) const { return {x * scalar, y * scalar}; }
    inline constexpr T operator*(const BasicPoint &other) const { return x * other.x + y * other.y; }
    inline constexpr BasicPoint operator/(const T &scalar) const { return {x / scalar, y / scalar}; }
    inline friend constexpr BasicPoint operator*(const T &scalar, const BasicPoint &p) { return p * scalar; }
    inline constexpr T length() const {
        return x == ZeroT ? math::abs(y) :
               y == ZeroT ? math::abs(x) : math::sqrt(x * x + y * y); // Improved numeric stability
    }
    inline constexpr T distance(const BasicPoint &other) const { return (*this - other).length(); }
    inline constexpr T operator|(const BasicPoint &other) const {
        return math::acos((*this * other) / (length() * other.length()));
    }
    inline constexpr T angle(const BasicPoint &other) const { return *this | other; }

    // Output
    inline void print() const { OUT << "(" << x << ", " << y << ")\n"; }
    inline friend std::ostream &operator<<(std::ostream &os, const BasicPoint &p) {
        os << "(" << p.x << ", " << p.y << ")";
        return os;
    }
};

using Point = BasicPoint<float>;
using Vector = Point;

#endif //BEZIER_POINT_H
