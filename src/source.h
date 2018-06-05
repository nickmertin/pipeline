//
// Created by nmeritn on 2018-06-03.
//

#ifndef PIPELINE_SOURCE_H
#define PIPELINE_SOURCE_H


#include <memory>
#include <unordered_set>
#include <vector>
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
        protected:
            source<T> *_source;

        private:
            std::function<void(T)> _push;
            std::function<void()> _unbind;

        public:
            explicit true_source_binding(source<T> *_source) : _source(_source) {}

            void bind(std::function<void(T)> push, std::function<void()> unbind) final {
                _push = push;
                _unbind = unbind;
                _source->bindings.insert(this);
            }

            void push(T value) {
                if (_push)
                    _push(value);
            }

            void unbind() {
                if (_unbind)
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

        class filter_start final : public source_binding<T> {
        public:
            std::function<void(T)> push;

            filter_start() : push() {}

            void bind(std::function<void(T)> push, std::function<void()> unbind) override {
                this->push = push;
            }
        };

        class filter_end final : public sink<T> {
        private:
            source<T> *_source;

        public:
            explicit filter_end(source<T> *_source) : _source(_source) {}

        protected:
            void accept(T value) override {
                _source->push_internal(value);
            }
        };

        std::unordered_set<true_source_binding *> bindings;
        std::vector<generic_filter<T>> filters;
        filter_start _filter_start;
        filter_end _filter_end;

    public:
        source() : _filter_end(this) {}

    private:
        void push_internal(T value) {
            for (true_source_binding *b : bindings) {
                b->push(value);
            }
        }

    protected:
        void push(T value) {
            if (filters.empty())
                push_internal(value);
            else
                _filter_start.push(value);
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

        template <class TFilter>
        source<T> &operator|=(const TFilter &_filter) {
            if (filters.empty()) {
                filters.push_back(_filter);
                filters.front().bind(&_filter_start);
                filters.front() | _filter_end;
            }
            else {
                auto &last = filters.back();
                filters.push_back(_filter);
                last | filters.back();
                filters.back() | _filter_end;
            }
        }

        void reset() {
            filters.clear();
        }

        virtual ~source() {
#ifdef PIPELINE_DEBUG
            std::cerr << "Destruct source " << typeid(*this).name() << " (" << std::hex << this << ")" << std::endl;
#endif // PIPELINE_DEBUG
            for (true_source_binding *b : bindings) {
                b->unbind();
            }
        }
    };
}


#endif //PIPELINE_SOURCE_H
