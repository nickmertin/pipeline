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
        class out_sink final : public sink<T> {
            generic_filter<T> *_filter;

        public:
            out_sink(generic_filter<T> *_filter) : _filter(_filter) {}

        protected:
            void accept(T value) override {
                _filter->push(value);
            }
        };

        filter<T> *_filter;
        out_sink _sink;

    protected:
        void accept(T value) override {

        }

    public:
        template <class TFilter>
        generic_filter(const TFilter &_filter) : _filter(new TFilter(_filter)), _sink(this) { // NOLINT
            *this->_filter | _sink;
        }

        generic_filter(std::function<T(T)> _function) : generic_filter(pipeline(_function)) {} // NOLINT
    };
}


#endif //PIPELINE_GENERIC_FILTER_H
