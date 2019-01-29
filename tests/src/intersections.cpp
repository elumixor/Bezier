//
// Created by Vladyslav Yazykov on 07/01/2019.
//

#include <gtest/gtest.h>
#include <Polygon.h>

// Polygon intersections

TEST(polygon_intersections, not_intersecting) {}

TEST(polygon_intersections, one_lies_completely_inside) {}

TEST(polygon_intersections, one_has_points_inside) {}

TEST(polygon_intersections, polygons_that_are_lines_intersections) {
    Polygon line1{{0, 0},
                  {1, 1}};
    Polygon line2{{1, 0},
                  {0, 1}};

    EXPECT_TRUE(line1.intersects(line2));
    EXPECT_TRUE(line1.intersects(line2));

    EXPECT_FALSE((Polygon{{0, 0},
                          {0, 1}}.intersects(Polygon{{1, 0},
                                                     {1, 1}})));
}

TEST(polygon_intersections, no_inside_points_but_intersect) {}

TEST(polygon_intersections, touching_edge_do_not_intersect) {}

TEST(polygon_intersections, touching_on_point_do_not_intersect) {}

TEST(polygon_intersections, points_touching_but_segments_intersect) {
    Polygon polygon1{{0, 0},
                     {5, 5},
                     {0, 10}},
            polygon2{{5, 0},
                     {0, 5},
                     {5, 10}};
    EXPECT_EQ(polygon1.intersects(polygon2), true);
}

// Curve intersections

TEST(curve_intersections, not_intersecting) {
}

TEST(curve_intersections, lines_intersect) {}

TEST(curve_intersections, lines_do_not_intersect) {}

TEST(curve_intersections, higher_order_curves) {

}

TEST(curve_intersections, same_curve_intersects_infinitely) {}

TEST(curve_intersections, same_curve_with_offset_do_not_intersect) {}

TEST(intersctions, same_control_points_different_order) {}