//
// Created by nmeritn on 2018-06-04.
//

#ifndef PIPELINE_AS_PIPELINE_H
#define PIPELINE_AS_PIPELINE_H


#include <type_traits>
#include "source.h"

namespace pipeline {
    enum pipeline_type : uint8_t {
        TYPE_NONE =     0b000,
        TYPE_SOURCE =   0b001,
        TYPE_SINK =     0b010,
        TYPE_FILTER =   0b100
    };

    template <class T, class TArg>
    class pipeline_type_of final {
    public:
        const static pipeline_type value = static_cast<pipeline_type>((std::is_base_of<source<T>, TArg>::value ? TYPE_SOURCE : TYPE_NONE) | (std::is_base_of<sink<T>, TArg>::value ? TYPE_SINK : TYPE_NONE) | (std::is_base_of<filter<T>, TArg>::value ? TYPE_FILTER : TYPE_NONE));
    };

    template <class T, class TArg, pipeline_type = pipeline_type_of<T, TArg>::value>
    class as_pipeline final {
    public:
        static const TArg &get(const TArg &arg) {
            return arg;
        }
    };

    template <class T, class TArg>
    class as_pipeline<T, TArg, TYPE_NONE> final {
    public:
        static auto get(const TArg &arg) {
            return pipeline(arg);
        }
    };
}


#endif //PIPELINE_AS_PIPELINE_H
