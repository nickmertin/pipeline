//
// Created by nmeritn on 2018-06-03.
//

#ifndef PIPELINE_BINDING_H
#define PIPELINE_BINDING_H


#include <memory>
#include "source_binding.h"
#include "sink_binding.h"

#ifdef PIPELINE_DEBUG
#include <iostream>
#include <typeinfo>
#endif // PIPELINE_DEBUG

namespace pipeline {
    template <class T>
    class binding final {
    private:
        std::unique_ptr<source_binding<T>> _source;
        std::unique_ptr<sink_binding<T>> _sink;
        bool destructing = false;

        void bind() {
            if (_source && _sink) {
                _source->bind([this](const T &value) { _sink->push(value); },
                              [this]() { if (!destructing) replace(nullptr, nullptr); });
#ifdef PIPELINE_DEBUG
                std::cerr << "Bound " << typeid(*_sink).name() << " (" << std::hex << &*_sink << ") to " << typeid(*_source).name() << " (" << std::hex << &*_source << ")" << std::endl;
#endif // PIPELINE_DEBUG
            }
        }

    public:
        binding(std::unique_ptr<source_binding<T>> _source, std::unique_ptr<sink_binding<T>> _sink) : _source(std::move(_source)), _sink(std::move(_sink)) {
            bind();
        }

        binding(source_binding<T> *_source, sink_binding<T> *_sink) : _source(_source), _sink(_sink) {
            bind();
        }

        binding() : binding(nullptr, nullptr) {}

        binding(const binding<T> &) = delete;

        binding<T> &operator=(const binding<T> &) = delete;

        void replace(std::unique_ptr<source_binding<T>> _source, std::unique_ptr<sink_binding<T>> _sink) {
            this->_source = std::move(_source);
            this->_sink = std::move(_sink);
            bind();
        }

        void replace(source_binding<T> *_source, sink_binding<T> *_sink) {
            replace(std::unique_ptr<source_binding<T>>(_source), std::unique_ptr<sink_binding<T>>(_sink));
        }

        ~binding() {
            destructing = true;
        }
    };
}


#endif //PIPELINE_BINDING_H
