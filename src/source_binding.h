//
// Created by nmeritn on 2018-06-03.
//

#ifndef PIPELINE_SOURCE_BINDING_H
#define PIPELINE_SOURCE_BINDING_H


#include <functional>

namespace pipeline {
    template <class T>
    class source_binding {
    public:
        virtual void bind(std::function<void(T)> push, std::function<void()> unbind) = 0;

        virtual ~source_binding() {}
    };
}


#endif //PIPELINE_SOURCE_BINDING_H
