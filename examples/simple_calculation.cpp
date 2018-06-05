//
// Created by nmeritn on 2018-06-03.
//

#define PIPELINE_DEBUG

#include <cmath>
#include <iostream>
#include "../src/generic_filter.h"

int main() {
    pipeline::generic_filter<double> filter((std::function<double(double)>) ::sin);
    filter |= (std::function<double(double)>) ::cos;
    filter | [] (double x) { std::cout << x << std::endl; };
    17.5 | filter;
    return 0;
}
