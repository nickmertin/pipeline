//
// Created by nmeritn on 2018-06-03.
//

#ifndef PIPELINE_FILTER_H
#define PIPELINE_FILTER_H


#include "source.h"
#include "sink.h"
#include "generic_filter.h"

namespace pipeline {
    template<class T>
    class generic_filter;

    template <class T>
    class filter : public source<T>, public sink<T> {
    public:
        filter() {}

        filter(filter<T> &&) noexcept = default;
    };
}


#endif //PIPELINE_FILTER_H
