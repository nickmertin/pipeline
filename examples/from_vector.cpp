//
// Created by nmeritn on 2018-06-05.
//

#include <vector>
#include <iostream>
#include "../src/generic_filter.h"
#include "../src/from.h"

int main() {
    std::vector<int> v{1, 2, 3};
    pipeline::generic_filter<int> filter([] (int x) { return x * x; });
    filter | [] (int x) { std::cout << x << std::endl; };
    pipeline::from(v) | filter;
}
