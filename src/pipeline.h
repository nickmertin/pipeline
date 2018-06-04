//
// Created by nmeritn on 2018-06-03.
//

#ifndef PIPELINE_PIPELINE_H
#define PIPELINE_PIPELINE_H


#include <functional>

namespace pipeline {
    template<class T, class U>
    class function_filter;

    template<class T, class U>
    function_filter<T, U> pipeline(std::function<U(T)> _function) {
        return function_filter<T, U>(_function);
    };
}

#endif //PIPELINE_PIPELINE_H
