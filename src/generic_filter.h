//
// Created by nmeritn on 2018-06-03.
//

#ifndef PIPELINE_GENERIC_FILTER_H
#define PIPELINE_GENERIC_FILTER_H


#include "filter.h"
#include "pipeline.h"
#include "function_filter.h"

namespace pipeline {
    template <class T>
    class generic_filter final : public filter<T> {
    private:
        class in_source final : public source<T> {
            generic_filter<T> *_filter;

        public:
            explicit in_source(generic_filter<T> *_filter) : _filter(_filter) {}

            void accept(T value) {
                this->push(value);
            }
        };

        class out_sink final : public sink<T> {
            generic_filter<T> *_filter;

        public:
            explicit out_sink(generic_filter<T> *_filter) : _filter(_filter) {}

        protected:
            void accept(T value) override {
                _filter->push(value);
            }
        };

        filter<T> *_filter;
        in_source _source;
        out_sink _sink;

    protected:
        void accept(T value) override {
            _source.accept(value);
        }

    public:
        template <class TFilter>
        generic_filter(const TFilter &_filter) : _filter(new TFilter(_filter)), _source(this), _sink(this) { // NOLINT
            _source | *this->_filter;
            *this->_filter | _sink;
        }

        generic_filter(std::function<T(T)> _function) : generic_filter(pipeline(_function)) {} // NOLINT
    };
}


#endif //PIPELINE_GENERIC_FILTER_H
