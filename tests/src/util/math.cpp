//
// Created by Vladyslav Yazykov on 06/01/2019.
//

#include <gtest/gtest.h>
#include <util/math.h>

#include <util/code_organizers.h>

TEST(math_test, test_eq) {
    EXPECT_EQ(1, 1);
}

TEST(math_test, test_neq) {
    EXPECT_NE(1, 0);
}