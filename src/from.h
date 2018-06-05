//
// Created by nmeritn on 2018-06-05.
//

#ifndef PIPELINE_FROM_H
#define PIPELINE_FROM_H


#include <type_traits>
#include "sink.h"

namespace pipeline {
    template <class T, class TContainer>
    auto &&from(const TContainer &container) {
        class result final {
            const TContainer &container;
        public:
            explicit result(const TContainer &container) : container(container) {}

            void operator |(sink<T> &_sink) {
                for (const std::remove_reference<T> &value : container) {
                    value | _sink;
                }
            }
        };

        return result(container);
    }
}


#endif //PIPELINE_FROM_H
