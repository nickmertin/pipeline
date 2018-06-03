//
// Created by nmeritn on 2018-06-03.
//

#ifndef PIPELINE_BINDING_H
#define PIPELINE_BINDING_H


#include <memory>
#include "source_binding.h"
#include "sink_binding.h"

namespace pipeline {
    template <class T>
    class binding final {
    private:
        std::unique_ptr<source_binding<T>> _source;
        std::unique_ptr<sink_binding<T>> _sink;

        void bind() {
            _source->bind([this] (const T &value) { _sink->push(value); }, [this] () { replace(nullptr, nullptr); });
        }

    public:
        binding(std::unique_ptr<source_binding<T>> _source, std::unique_ptr<sink_binding<T>> _sink) : _source(std::move(_source)), _sink(std::move(_sink)) {
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

        virtual ~binding() {
            delete _source;
            delete _sink;
        }
    };
}


#endif //PIPELINE_BINDING_H
