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

    template<typename T>
    constexpr T precision();
    template<>
    constexpr float precision<float>() { return PRECISION_FLOAT; }
    template<>
    constexpr double precision<double>() { return PRECISION_DOUBLE; }

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

    template<typename T>
    constexpr T pow(T num, size_t _pow) {
        return (_pow >= sizeof(size_t) * 8) ? 0 :
               _pow == 0 ? 1 : num * pow(num, _pow - 1);
    }


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

    template<typename T>
    constexpr T min(T a, T b) {
        return a > b ? b : a;
    }

    template<typename T>
    constexpr T max(T a, T b) {
        return a < b ? b : a;
    }

    template<typename T>
    constexpr T abs(T value) {
        return value < 0 ? -value : value;
    }

    template<typename T>
    constexpr bool equal(T a, T b) {
        return abs(a - b) < precision<T>();
    }

    //region Constexpr square root from https://gist.github.com/kimwalisch/d249cf684a58e1d892e1
#define MID ((lo + hi + 1) / 2)
    template<typename T>
    constexpr T sqrt_helper(T x, T lo, T hi) {
        return (equal<T>(x / MID, MID) || equal<T>(lo, hi)) ? lo :
               ((x / MID < MID) ? sqrt_helper(x, lo, MID - 1) : sqrt_helper(x, MID, hi));
    }

    template<>
    constexpr int sqrt_helper<int>(int x, int lo, int hi) {
        return lo == hi ? lo :
               ((x / MID < MID) ? sqrt_helper(x, lo, MID - 1) : sqrt_helper(x, MID, hi));
    }

    template<typename T>
    constexpr T sqrt(T x) {
        return sqrt_helper<T>(x, 0, x / 2 + 1);
    }

#undef MID
    //endregion

    //region Constexpr acos, asin from https://github.com/elbeno/constexpr

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

    template<typename T>
    constexpr T acos(T x) {
        return equal<T>(x, -1) ? math::pi<T>() :
               equal<T>(x, 1) ? 0 :
               math::pi<T>() / T{2} - math::asin<T>(x);
    }
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
