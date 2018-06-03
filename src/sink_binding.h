//
// Created by nmeritn on 2018-06-03.
//

#ifndef PIPELINE_SINK_BINDING_H
#define PIPELINE_SINK_BINDING_H


namespace pipeline {
    template <class T>
    class sink_binding {
    public:
        virtual void push(T value) = 0;
    };
}


#endif //PIPELINE_SINK_BINDING_H
