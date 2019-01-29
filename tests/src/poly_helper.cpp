//
// Created by Vladyslav Yazykov on 07/01/2019.
//

#include <gtest/gtest.h>
#include <Polygon.h>

// Poly nodes

TEST(poly_helper_node, constructors_and_assignments) {}

TEST(poly_helper_node, memory_freeing) {}

TEST(poly_helper_node, iterating) {}

TEST(poly_helper_node, addition_and_removal) {}

// Orientation
TEST(poly_helper, orientation_basic) {
    EXPECT_EQ(poly_helper::orientation({0, 0}, {1, 1}, {1, 0}), poly_helper::Clockwise);
    EXPECT_EQ(poly_helper::orientation({0, 0}, {1, 1}, {0, 1}), poly_helper::Counterclockwise);
    EXPECT_EQ(poly_helper::orientation({0, 0}, {1, 1}, {.5, .5}), poly_helper::Collinear);
}

TEST(poly_helper, orientation_clockwise) {
    EXPECT_EQ(poly_helper::orientation({0, 0}, {1, 1}, {1, 0}), poly_helper::Clockwise);
    EXPECT_EQ(poly_helper::orientation({0, 0}, {1, 1}, {0, 1}), poly_helper::Counterclockwise);
    EXPECT_EQ(poly_helper::orientation({0, 0}, {1, 1}, {.5, .5}), poly_helper::Collinear);
}

TEST(poly_helper, orientation_counterclockwise) {}

TEST(poly_helper, orientation_collinear) {}

// Compute Angle

TEST(poly_helper, angles_are_correct) {}

// Segment intersections

TEST(poly_helper, cross_intersecting) {
    EXPECT_TRUE(poly_helper::segments_intersect({0, 0}, {1, 1}, {1, 0}, {0, 1}));
}

TEST(poly_helper, touching_do_not_intersect) {}

TEST(poly_helper, collinear_do_not_intersect) {}

TEST(poly_helper, not_collinear_but_far_away_do_not_intersect) {}

// Form path from bottom left point to top right (takes a sorted array)

TEST(poly_helper, path_from_simple_line) {}

TEST(poly_helper, path_from_one_point) {}

TEST(poly_helper, path_to_bottom_is_not_formed) {}

TEST(poly_helper, concave_point_is_ignored) {}

TEST(poly_helper, convex_point_is_added) {}

TEST(poly_helper, remove_previous_point_on_too_high_point) {}

TEST(poly_helper, remove_points_til_beginning) {}

TEST(poly_helper, remove_several_previous_points_on_too_high_point) {}

