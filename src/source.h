//
// Created by nmeritn on 2018-06-03.
//

#ifndef PIPELINE_SOURCE_H
#define PIPELINE_SOURCE_H


#include <memory>
#include <unordered_set>
#include "source_binding.h"
#include "sink.h"

namespace pipeline {
    template <class T>
    class filter;

    template <class T>
    class generic_filter;

    template <class T>
    class source {
    private:
        class true_source_binding : public source_binding<T> {
        private:
            source<T> *_source;
            std::function<void(T)> _push;
            std::function<void()> _unbind;

        public:
            explicit true_source_binding(source<T> *_source) : _source(_source) {}

            void bind(std::function<void(T)> push, std::function<void()> unbind) override final {
                _push = push;
                _unbind = unbind;
                _source->bindings.insert(this);
            }

            void push(T value) {
                _push(value);
            }

            void unbind() {
                _unbind();
            }

            virtual ~true_source_binding() {
                _source->bindings.erase(this);
            }
        };

        class internal_source_binding final : public true_source_binding {
        private:
            binding<T> _binding;

        public:
            internal_source_binding(source<T> *_source, sink_binding<T> *_sink) : true_source_binding(_source), _binding(dynamic_cast<source_binding<T> *>(this), _sink) {}
        };

        std::unordered_set<true_source_binding *> bindings;

        generic_filter<T> *_filter = nullptr;

        void push_internal(T value) {
            for (true_source_binding *b : bindings) {
                b->push(value);
            }
        }

        class filter_out_binding final : public sink_binding<T> {
        private:
            source<T> *_source;

        public:
            explicit filter_out_binding(source<T> *_source) : _source(_source) {}

            void push(T value) override {
                _source->push_internal(value);
            }
        };

        binding<T> *out_binding;

    protected:
        void push(T value) {
            if (_filter)
                _filter->push(value);
            else
                push_internal(value);
        }

    public:
        void operator|(sink<T> &_sink) {
            _sink.bind(new true_source_binding(this));
        }

        void operator|(std::function<void(T)> _sink) {
            class function_sink final : public sink_binding<T> {
            private:
                std::function<void(T)> _function;

            public:
                function_sink(const std::function<void(T)> &_function) : _function(_function) {}

            protected:
                void push(T value) override {
                    _function(value);
                }
            };

            new internal_source_binding(this, new function_sink(_sink));
        }

        source<T> &operator|=(filter<T> &_filter) {
            generic_filter<T> *old = this->_filter;
            this->_filter = old ? new generic_filter<T>(*old | _filter) : new generic_filter<T>(_filter);
            delete out_binding;
            out_binding = new binding<T>(new true_source_binding(this->_filter), new filter_out_binding(this));
        }

        void reset() {
            delete _filter;
            _filter = nullptr;
        }

        virtual ~source() {
            for (true_source_binding *b : bindings) {
                b->unbind();
            }
        }
    };
}


#endif //PIPELINE_SOURCE_H
