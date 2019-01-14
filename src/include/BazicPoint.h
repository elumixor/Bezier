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

    static constexpr T crop_angle(const T angle) {
        return (angle == math::pi<T>() || angle == -math::pi<T>()) ? ZeroT :
               angle < -math::pi<T>() ?
               angle + math::pi<T>() * 2.0 :
               angle > math::pi<T>() ?
               angle - math::pi<T>() * 2.0 :
               angle;
    }
public:
    /// Coords
    T x{}, y{};

    // Constructors
    inline constexpr BasicPoint() noexcept = default;
    inline constexpr explicit BasicPoint(T val) noexcept : x{val}, y{val} {}
    inline constexpr BasicPoint(T x, T y) noexcept : x{x}, y{y} {}
    inline constexpr BasicPoint(const BasicPoint &other) noexcept  : x{other.x}, y{other.y} {}
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

    // Comparisons
    inline constexpr bool operator==(const BasicPoint &other) const { return x == other.x && y == other.y; }
    inline constexpr bool operator!=(const BasicPoint &other) const { return !(*this == other); }
    inline constexpr bool operator>(const BasicPoint &other) const { return length() > other.length(); }
    inline constexpr bool operator>=(const BasicPoint &other) const { return length() >= other.length(); }
    inline constexpr bool operator<(const BasicPoint &other) const { return length() < other.length(); }
    inline constexpr bool operator<=(const BasicPoint &other) const { return length() <= other.length(); }

    // Arithmetic operations
    inline constexpr BasicPoint operator+(const BasicPoint &other) const { return {x + other.x, y + other.y}; }
    inline constexpr BasicPoint &operator+=(const BasicPoint &other) const {
        x += other.x;
        y += other.y;
        return *this;
    }

     inline constexpr BasicPoint operator-(const BasicPoint &other) const { return {x - other.x, y - other.y}; }
    inline constexpr BasicPoint &operator-=(const BasicPoint &other) const {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    inline constexpr BasicPoint operator*(const T &scalar) const { return {x * scalar, y * scalar}; }
    inline constexpr T operator*(const BasicPoint &other) const { return x * other.x + y * other.y; }
    inline constexpr BasicPoint &operator*=(const T &scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    inline friend constexpr BasicPoint operator*(const T &scalar, const BasicPoint &p) { return p * scalar; }

    inline constexpr BasicPoint operator/(const T &scalar) const { return {x / scalar, y / scalar}; }

    // Vector-like operations
    inline T operator|(const BasicPoint &other) const {
        return crop_angle(atan2(other.y, other.x) - atan2(y, x));
    }
    inline T angle(const BasicPoint &other) const noexcept { return *this | other; }

    inline constexpr T length() const {
        return x == ZeroT ? math::abs(y) :
               y == ZeroT ? math::abs(x) : math::sqrt(x * x + y * y); // Improved numeric stability
    }
    inline constexpr T distance(const BasicPoint &other) const { return (*this - other).length(); }

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
