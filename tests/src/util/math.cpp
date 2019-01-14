//
// Created by Vladyslav Yazykov on 06/01/2019.
//

#include <gtest/gtest.h>
#include <util/math.h>

#include <util/code_organizers.h>

#define MTEST(name) TEST(math_test, test_ ## name)

MTEST(constexpr) {
    constexpr auto factorial{math::factorial(6)}; // Factorial is constexpr
    static_cast<void>(factorial); // Silence unused variable

    constexpr auto range_product{math::range_product(6, 8)}; // Factorial is constexpr
    static_cast<void>(range_product);

    constexpr auto min{math::min(6, 8)}; // Factorial is constexpr
    static_cast<void>(min);

    constexpr auto max{math::max(6, 8)}; // Factorial is constexpr
    static_cast<void>(max);

    constexpr auto abs{math::abs(6)}; // Factorial is constexpr
    static_cast<void>(abs);


    constexpr bool equal{math::equal<int>(6, 8)};
    constexpr bool equal_f{math::equal<float>(6, 8)};
    constexpr bool equal_d{math::equal<double>(6, 8)};

    static_cast<void>(equal);
    static_cast<void>(equal_f);
    static_cast<void>(equal_d);

    constexpr auto modulo{math::modulo(6, 8)};
    constexpr auto modulo_f{math::modulo<float>(6, 8)};
    constexpr auto modulo_d{math::modulo<double>(6, 8)};

    static_cast<void>(modulo);
    static_cast<void>(modulo_f);
    static_cast<void>(modulo_d);

    constexpr auto sign{math::sign(6)};
    static_cast<void>(sign);

    constexpr auto pow{math::pow(6, 8)};
    static_cast<void> (pow);

    constexpr auto sqrt{math::sqrt(6)};
    static_cast<void> (sqrt);

    constexpr auto asin{math::asin(0.6)};
    static_cast<void> (asin);

    constexpr auto acos{math::acos(0.6)};
    static_cast<void> (acos);

    constexpr auto atan{math::atan(6)};
    static_cast<void> (atan);

    constexpr auto atan2{math::atan2(6, 8)};
    static_cast<void> (atan2);

    constexpr auto degrees{math::degrees(6)};
    static_cast<void> (degrees);

    constexpr auto radians{math::radians(6)};
    static_cast<void> (radians);
}

/// Factorial tests
MTEST(factorial) {
    EXPECT_EQ(math::factorial(0), 1);
    EXPECT_EQ(math::factorial(1), 1);
    EXPECT_EQ(math::factorial(-1), 1);

    EXPECT_EQ(math::factorial(2), 2);
    EXPECT_EQ(math::factorial(3), 2 * 3);
    EXPECT_EQ(math::factorial(5), 2 * 3 * 4 * 5);

    EXPECT_EQ(math::factorial(7), math::factorial(5) * 6 * 7);


    for (size_t i{0}; i < 100; i++) {
        auto a = math::random_int(2, 30);
        int product{1};
        for (int j{2}; j <= a; j++) product *= j;

        EXPECT_EQ(product, math::factorial(a));
    }
}

/// Range product
MTEST(range_product) {
    EXPECT_EQ(math::range_product(0, 0), 0);
    EXPECT_EQ(math::range_product(-1, 0), 0);
    EXPECT_EQ(math::range_product(-1, 1), 0);
    EXPECT_EQ(math::range_product(-2, -1), 2);
    EXPECT_EQ(math::range_product(1, 2), 2);
    EXPECT_EQ(math::range_product(1, 5), 1 * 2 * 3 * 4 * 5);
    EXPECT_EQ(math::range_product(1, 5), math::factorial(5));
    EXPECT_EQ(math::range_product(3, 7), 3 * 4 * 5 * 6 * 7);
    EXPECT_EQ(math::range_product(8, 7), 1);
    EXPECT_EQ(math::range_product(100, 7), 1);
    EXPECT_EQ(math::range_product(100, -100), 1);

    EXPECT_EQ(math::range_product(5, 10), math::factorial(10) / math::factorial(10 - 6));

    for (size_t i{0}; i < 100; i++) {
        auto a = math::random_int(5, 10);
        auto b = math::random_int(1, a);
        EXPECT_EQ(math::factorial(a) / math::factorial(b), math::range_product(b + 1, a));
    }
}

/// Min
MTEST(min) {
    EXPECT_EQ(math::min(1, 3), 1);
    EXPECT_EQ(math::min(-1, 3), -1);
    EXPECT_EQ(math::min(-1, -3), -3);
    EXPECT_EQ(math::min(-1, 0), -1);
    EXPECT_EQ(math::min(11, 0), 0);
    EXPECT_EQ(math::min(0, 0), 0);
    EXPECT_EQ(math::min(10, 10), 10);

    EXPECT_EQ(math::min(.1, .01), .01);
    EXPECT_EQ(math::min(-.1, .01), -.1);
    EXPECT_EQ(math::min(0.000000001, .0000000001), .0000000001);

    EXPECT_EQ(math::min(.1f, .01f), .01f);
    EXPECT_EQ(math::min(-.1f, .01f), -.1f);
    EXPECT_EQ(math::min(0.000000001f, .0000000001f), .0000000001f);
}

/// Max
MTEST(max) {
    EXPECT_EQ(math::max(1, 3), 3);
    EXPECT_EQ(math::max(-1, 3), 3);
    EXPECT_EQ(math::max(-1, -3), -1);
    EXPECT_EQ(math::max(-1, 0), 0);
    EXPECT_EQ(math::max(11, 0), 11);
    EXPECT_EQ(math::max(0, 0), 0);
    EXPECT_EQ(math::max(10, 10), 10);

    EXPECT_EQ(math::max(.1, .01), .1);
    EXPECT_EQ(math::max(-.1, .01), .01);
    EXPECT_EQ(math::max(0.000000001, .0000000001), .000000001);

    EXPECT_EQ(math::max(.1f, .01f), .1f);
    EXPECT_EQ(math::max(-.1f, .01f), .01f);
    EXPECT_EQ(math::max(0.000000001f, .0000000001f), .000000001f);
}

/// Absolute
MTEST(abs) {
    EXPECT_EQ(math::abs(0), 0);
    EXPECT_EQ(math::abs(1), 1);
    EXPECT_EQ(math::abs(-1), 1);

    EXPECT_EQ(math::abs(-1.111), 1.111);
    EXPECT_EQ(math::abs(1.111), 1.111);

    EXPECT_EQ(math::abs(-1.111f), 1.111f);
    EXPECT_EQ(math::abs(1.111f), 1.111f);
}

/// Equal
MTEST(equal) {
    EXPECT_EQ(math::equal(1, 0), false);
    EXPECT_EQ(math::equal(1, 1), true);
    EXPECT_EQ(math::equal(0, 0), true);
    EXPECT_EQ(math::equal(1, -1), false);


    EXPECT_EQ(math::equal<double>(1. / 100, 1. / 200), false);
    EXPECT_EQ(math::equal<float>(-1.f / 1000000, -1.f / 1000000000), true); // Precision is too small
    EXPECT_EQ(math::equal<double>(-1. / 1000000, -1. / 1000000000), false); // Precision is ok
    EXPECT_EQ(math::equal<double>(1. / 500, 1. / 500), true);
}

/// Modulo division
MTEST(modulo) {
    EXPECT_EQ(math::modulo(5, 3), 2);
    EXPECT_EQ(math::modulo<double>(5.2, 3), 2.2);
    EXPECT_EQ(math::modulo<double>(5.2, 0), 5.2);
    EXPECT_EQ(math::modulo(5, 1), 0);
    EXPECT_EQ(math::equal<double>(math::modulo<double>(5.2, 1), 0.2), true);
    EXPECT_NEAR(math::modulo<double>(5.2, 1), 0.2, math::precision<double>());
    EXPECT_EQ(math::equal<float>(math::modulo<float>(5.2, 1), 0.2), true);
    EXPECT_NEAR(math::modulo<float>(5.2, 1), 0.2, math::precision<float>());

    EXPECT_EQ(math::modulo(-math::pi(), math::pi()), 0.f);
    EXPECT_EQ(math::modulo(math::degrees(-math::pi()),math::degrees(math::pi())), 0.f);
}

/// Sign
MTEST(sign) {
    EXPECT_EQ(math::sign(0), 1);
    EXPECT_EQ(math::sign(1), -1);
    EXPECT_EQ(math::sign(2), 1);
    EXPECT_EQ(math::sign(-1), -1);
    EXPECT_EQ(math::sign(-2), 1);
}

/// Power
MTEST(pow) {
    EXPECT_EQ(math::pow(2, 2), 4);
    EXPECT_EQ(math::pow(2.2, 2), 2.2 * 2.2);
    EXPECT_EQ(math::pow(2.2, 2), 2.2 * 2.2);
    EXPECT_EQ(math::pow(-2.2, 2), 2.2 * 2.2);
    EXPECT_EQ(math::pow(-2.2, 2), -2.2 * -2.2);
    EXPECT_EQ(math::pow(1, 64), 1);
    EXPECT_EQ(math::pow(0, 64), 0);
    EXPECT_EQ(math::pow(0, 0), 1);
    EXPECT_EQ(math::pow(-1, 1), -1);
    EXPECT_EQ(math::pow(-1, 6), math::sign(6));
}

/// Square root
MTEST(sqrt) {
    EXPECT_EQ(math::sqrt(4), 2);
    EXPECT_NEAR(math::sqrt<double>(2.2 * 2.2), 2.2, math::precision<double>());
    EXPECT_NEAR(math::sqrt<float>(2.2f * 2.2f), 2.2f, math::precision<float>());
    EXPECT_EQ(math::sqrt(1), 1);
    EXPECT_EQ(math::sqrt(0), 0);
}

/// Radians and degrees
MTEST(degrees_and_radians) {
    for (int i{0}; i <= 90; i += 1) {
        EXPECT_NEAR(math::degrees(math::radians<float>(i)), i, 10 * math::precision<float>());
        EXPECT_NEAR(math::degrees(math::radians<double>(i)), i, 10 * math::precision<double>());
    }
}
