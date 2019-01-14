//
// Created by Vladyslav Yazykov on 07/01/2019.
//

#include <gtest/gtest.h>
#include <BazicPoint.h>

TEST(point, point_is_constexpr) {
    constexpr Point p{};
    static_cast<void> (p);
}

TEST(point, comparisons) {
    EXPECT_EQ(Point(), Point(0, 0));
    EXPECT_EQ(Point(0), Point(0, 0));
    EXPECT_EQ(Point(1), Point(1, 1));
    EXPECT_EQ(Point(1, 1), Point(1, 1));

    EXPECT_NE(Point(1, 1), Point(1, 2));
    EXPECT_NE(Point(1, 1), Point(2, 1));

    EXPECT_LE(Point(), Point());
    EXPECT_GE(Point(), Point());

    EXPECT_GE(Point(1), Point());
    EXPECT_GE(Point(1, 2), Point(1, 1));
    EXPECT_GE(Point(2, 1), Point(1, 1));
    EXPECT_GE(Point(4, 4), Point(5, 1));
    EXPECT_GE(Point(4, 4), Point(1, 5));
}

TEST(point, constructors) {

}

TEST(point, assignments) {}

TEST(point, arithmetic) {}

TEST(point, distance) {}

TEST(vector, dot_product) {}

TEST(vector, length) {}

TEST(vector, angles) {
    // Clockwise
    EXPECT_NEAR(Vector(1, 1) | Vector(1, 0), math::radians(45.f), math::precision());
    EXPECT_NEAR(Vector(0, 1) | Vector(1, 0), math::radians(90.f), math::precision());
    EXPECT_NEAR(Vector(-1, 1) | Vector(1, 0), math::radians(135.f), math::precision());

    // Collinear
    EXPECT_NEAR(Vector(-1, 0) | Vector(1, 0), math::radians(0.f), math::precision());
    EXPECT_NEAR(Vector(1, 0) | Vector(1, 0), math::radians(0.f), math::precision());

    // Counterclockwise
    EXPECT_NEAR(Vector(-1, -1) | Vector(1, 0), math::radians(-135.f), math::precision());
    EXPECT_NEAR(Vector(1, -1) | Vector(1, 0), math::radians(-45.f), math::precision());

    EXPECT_NEAR(Vector(1, -1) | Vector(1, 1), math::radians(-90.f), math::precision());
    EXPECT_NEAR(Vector(1, 1) | Vector(-1, 1), math::radians(-90.f), math::precision());

    EXPECT_NEAR(Vector(0, -1) | Vector(1, 0), math::radians(-90.f), math::precision());
    EXPECT_LE(Vector(0, -1) | Vector(1, 0), 0);
    EXPECT_LE(Vector(0, -1) | Vector(1, -1), 0);
    EXPECT_LE(Vector(0, -1) | Vector(.5, -1), 0);
    EXPECT_LE(Vector(-1, -1) | Vector(0, -1), 0);
    EXPECT_LE(Vector(-1, 1) | Vector(0, -1), 0);
    EXPECT_GE(Vector(1, 1) | Vector(0, -1), 0);
    EXPECT_LE(Vector(.5, -1) | Vector(1, -1), 0);
}