//
// Created by Vladyslav Yazykov on 17/12/2018.
//

#ifndef BEZIER_MATH_H
#define BEZIER_MATH_H

#include <random>
#include "float.h"

#define PRECISION_FLOAT 0.000001f
#define PRECISION_DOUBLE 0.00000000000000001
#define PI 3.14159265358979323846264338327950288

namespace math {
    template<typename T>
    inline constexpr T pi() { return T{PI}; };
    template<>
    inline constexpr int pi<int>() { return 3; };

    template<typename T>
    constexpr T precision();
    template<>
    constexpr float precision<float>() { return PRECISION_FLOAT; }
    template<>
    constexpr double precision<double>() { return PRECISION_DOUBLE; }

    //region Factorial

    template<int N>
    struct TFactorial {
        enum {
            value = N * TFactorial<N - 1>::value
        };
    };

    template<>
    struct TFactorial<0> {
        enum {
            value = 1
        };
    };

    template<typename T>
    constexpr int factorial(T v) {
        return v <= 0 ? 1 : v * factorial(v - 1);
    }
    //endregion

    //region Range product

    template<int N, int K>
    struct TRangeProduct {
        enum {
            value = N * TRangeProduct<N + 1, K>::value
        };
    };

    template<int K>
    struct TRangeProduct<K, K> {
        enum {
            value = K
        };
    };

    template<int K>
    struct TRangeProduct<K + 1, K> {
        enum {
            value = 1
        };
    };

    template<typename T>
    constexpr int range_product(T from, T to) {
        return from > to ? 1 : from == to ? from : from * range_product(from + 1, to);
    }
    //endregion

    //region Basic math functions

    template<typename T>
    constexpr T min(T a, T b) { return a > b ? b : a; }

    template<typename T>
    constexpr T max(T a, T b) { return a < b ? b : a; }

    template<typename T>
    constexpr T abs(T value) { return value < 0 ? -value : value; }

    template<typename T>
    constexpr bool equal(T a, T b) { return abs(a - b) < precision<T>(); }

    template<typename T>
    constexpr T modulo(T x, T y) { return x - (int) (x / y) * y; }

    //region Signum function

    template<int power>
    struct TSign {
        enum {
            value = power % 2 == 0 ? 1 : -1
        };
    };

    template<typename T>
    constexpr int sign(T value) {
        return value % 2 == 0 ? 1 : -1;
    }
    //endregion

    template<typename T>
    constexpr T pow(T num, size_t _pow) {
        return (_pow >= sizeof(size_t) * 8) ? 0 : _pow == 0 ? 1 : num * pow(num, _pow - 1);
    }
    //endregion

    //region Constexpr square root from https://gist.github.com/kimwalisch/d249cf684a58e1d892e1 with edits
#define MID ((lo + hi + 1) / 2)
    template<typename T>
    constexpr T sqrt_helper(T x, T lo, T hi, T prev_lo, T prev_hi, bool first) {
        return (equal<T>(x / MID, MID) || equal<T>(lo, hi) || (!first && (equal<T>(prev_hi, hi) && equal(prev_lo, lo))))
               ? lo :
               ((x / MID < MID) ? sqrt_helper(x, lo, MID - 1, lo, hi, false) : sqrt_helper(x, MID, hi, lo, hi, false));
    }

    template<>
    constexpr int sqrt_helper<int>(int x, int lo, int hi, int prev_lo, int prev_hi, bool first) {
        return (lo == hi || (!first && (prev_hi == hi && prev_lo == lo))) ? lo :
               ((x / MID < MID) ? sqrt_helper(x, lo, MID - 1, lo, hi, false) : sqrt_helper(x, MID, hi, lo, hi, false));
    }

    template<typename T>
    constexpr T sqrt(T x) {
        return sqrt_helper<T>(x, 0, x / 2 + 1, 0, 0, true);
    }

#undef MID
    //endregion

    //region Constexpr trigonometric functions from https://github.com/elbeno/constexpr

    //region Asin

    template<typename T>
    constexpr T asin_series(T x, T sum, int n, T t) {
        return equal(sum, sum + t * static_cast<T>(n) / (n + 2)) ?
               sum :
               asin_series(x, sum + t * static_cast<T>(n) / (n + 2), n + 2,
                           t * x * x * static_cast<T>(n) / (n + 3));
    }

    template<typename T>
    constexpr T asin(T x) {
        return equal<T>(x, -1) ? math::pi<T>() / T{-2} :
               equal<T>(x, 1) ? math::pi<T>() / T{2} :
               math::asin_series(x, x, 1, x * x * x / T{2});
    }
    //endregion

    //region Acos

    template<typename T>
    constexpr T acos(T x) {
        return equal<T>(x, -1) ? math::pi<T>() :
               equal<T>(x, 1) ? 0 :
               math::pi<T>() / T{2} - math::asin<T>(x);
    }
    //endregion

    //region Atan

    template<typename T>
    constexpr T atan_term(T x2, int k) {
        return (T{2} * static_cast<T>(k) * x2)
               / ((T{2} * static_cast<T>(k) + T{1}) * (T{1} + x2));
    }
    template<typename T>
    constexpr T atan_product(T x, int k) {
        return k == 1 ? atan_term(x * x, k) : atan_term(x * x, k) * atan_product(x, k - 1);
    }

    template<typename T>
    constexpr T atan_sum(T x, T sum, int n) {
        return sum + atan_product(x, n) == sum ?
               sum :
               atan_sum(x, sum + atan_product(x, n), n + 1);
    }

    template<typename T>
    constexpr T atan(T x) {
        return x / (T{1} + x * x) * math::atan_sum(x, T{1}, 1);
    }

    template<typename T>
    constexpr T atan2(T x, T y) {
        return modulo<T>(x > 0 ? math::atan(y / x) :
               y >= 0 && x < 0 ? math::atan(y / x) + math::pi<T>() :
               y < 0 && x < 0 ? math::atan(y / x) - math::pi<T>() :
               y > 0 && x == 0 ? (math::pi<T>() / 2.0) :
               -(math::pi<T>() / 2.0), pi<T>());
    }
    //endregion

    //endregion

    template<typename T>
    constexpr T degrees(T radians) { return radians * 180 / pi<T>(); }
    template<typename T>
    constexpr T radians(T degrees) { return degrees * pi<T>() / 180; }

    float random_float(float from = FLT_MIN / 1000.f, float to = FLT_MAX / 1000.f);
    int random_int(int from, int to);
}


#undef PRECISION_FLOAT
#undef PRECISION_DOUBLE
#undef PI

#endif //BEZIER_POW_H
