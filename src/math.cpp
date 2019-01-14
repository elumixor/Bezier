//
// Created by Vladyslav Yazykov on 26/12/2018.
//

#include "util/math.h"

int math::random_int(int from, int to) {
    static std::mt19937 mt{std::random_device()()};
    std::uniform_int_distribution<int> dist(from, to);
    return dist(mt);
}
float math::random_float(float from, float to) {
    static std::mt19937 mt{std::random_device()()};
    static std::uniform_real_distribution<> dist(from, to);
    return (float) dist(mt);
}


