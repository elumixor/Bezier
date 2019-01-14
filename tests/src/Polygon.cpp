//
// Created by Vladyslav Yazykov on 07/01/2019.
//

#include <gtest/gtest.h>
#include <Polygon.h>

TEST(polygon, from_no_redundant_points) {

}

TEST(polygon, from_points_on_same_lines) {
    Polygon p{{0, 0}, {1, 1}};
    Point points[]{{0, 0}, {0, 0}};

    EXPECT_THROW({
                     Polygon pp(points, 2);
                 }, std::invalid_argument);

    EXPECT_THROW(({
                     Polygon pp{{0,0}, {0,0}, {0,0}};
                 }), std::invalid_argument);

}

TEST(polygon, bounding_points_less_than_four) {}

TEST(polygon, from_colliner_points) {}

TEST(polygon, copy_and_assignment_operators) {}

TEST(polygon, from_curve) {}