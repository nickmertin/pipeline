//
// Created by nmeritn on 2018-06-03.
//

#ifndef PIPELINE_FUNCTION_FILTER_H
#define PIPELINE_FUNCTION_FILTER_H


#include "filter.h"

namespace pipeline {
    template<class T, class U>
    class function_filter final : public source<U>, public sink<T> {
    private:
        std::function<U(T)> _function;

    public:
        function_filter(const function_filter<T, U> &other) : _function(other._function) {}

        explicit function_filter(const std::function<U(T)> &_function) : _function(_function) {}

    private:
        void accept(T value) override {
            push(_function(value));
        }
    };

    template <class T>
    class function_filter<T, T> final : public filter<T> {
    private:
        std::function<T(T)> _function;

    public:
        function_filter(const function_filter<T, T> &other) : _function(other._function) {}

        explicit function_filter(const std::function<T(T)> &_function) : _function(_function) {}

    private:
        void accept(T value) override {
            this->push(_function(value));
        }
    };
}


#endif //PIPELINE_FUNCTION_FILTER_H
