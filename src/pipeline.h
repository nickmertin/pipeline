//
// Created by nmeritn on 2018-06-03.
//

#ifndef PIPELINE_PIPELINE_H
#define PIPELINE_PIPELINE_H


#include <functional>

namespace pipeline {
    template<class T, class U>
    class function_filter;

    template <class T>
    class filter;

    template <bool is_filter>
    class pipeline_impl;

    template<class T, class U>
    function_filter<T, U> pipeline(std::function<U(T)> _function) {
        return function_filter<T, U>(_function);
    };

    template <class T, class U>
    function_filter<T, U> pipeline(U (*_function)(T)) {
        std::function<U(T)> f = _function;
        return function_filter<T, U>(f);
    };

    template <class T>
    const filter<T> &pipeline(const filter<T> &_filter) {
        return _filter;
    }
}

#endif //PIPELINE_PIPELINE_H
