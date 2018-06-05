//
// Created by nmeritn on 2018-06-05.
//

#include <iostream>
#include "../src/source.h"

template <class T>
class stream_source final : public pipeline::source<T> {
    std::istream &in;

public:
    explicit stream_source(std::istream &in) : in(in) {}

    void run(std::function<bool(T)> condition) {
        T value;
        do {
            in >> value;
            this->push(value);
        } while (condition(value));
    }
};

int main() {
    stream_source<std::string> source(std::cin);
    source | [] (const std::string &str) {
        std::cout << str << std::endl;
    };
    source.run([] (const std::string &str) { return !str.empty(); });
}
