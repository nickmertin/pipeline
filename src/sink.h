//
// Created by nmeritn on 2018-06-03.
//

#ifndef PIPELINE_SINK_H
#define PIPELINE_SINK_H


#include <memory>
#include "binding.h"

namespace pipeline {
    template <class T>
    class sink {
    private:
        class true_sink_binding final : public sink_binding<T> {
        private:
            sink<T> *_sink;

        public:
            explicit true_sink_binding(sink<T> *_sink) : _sink(_sink) {}
            
            void push(T value) override {
                _sink->accept(value);
            }
        };

        binding<T> _binding;

    protected:
        virtual void accept(T value) = 0;

    public:
        void unbind() {
            _binding.replace(nullptr, nullptr);
        }

        void bind(source_binding<T> *_source) {
            if (_source)
                _binding.replace(_source, new true_sink_binding(this));
            else
                unbind();
        }

        friend void operator|(T value, sink<T> &_sink) {
            _sink.unbind();
            _sink.accept(value);
        }

        virtual ~sink() {
            unbind();
        }
    };
}


#endif //PIPELINE_SINK_H
