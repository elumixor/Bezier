//
// Created by Vladyslav Yazykov on 26/12/2018.
//

#ifndef BEZIER_LOGIC_H
#define BEZIER_LOGIC_H

#include <iostream>
#include "Bezier.h"

class Params {
    static constexpr size_t CURVES_COUNT{1000};
    static constexpr size_t POINTS_MIN{2};
    static constexpr size_t POINTS_MAX{6};
public:
    size_t curves_count{CURVES_COUNT};
    size_t points_min{POINTS_MIN};
    size_t points_max{POINTS_MAX};
};

int run_test();

/// Generate bezier curves
int generate(const Params &);

#endif //BEZIER_LOGIC_H
