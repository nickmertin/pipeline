//
// Created by nmeritn on 2018-06-03.
//

#ifndef PIPELINE_GENERIC_FILTER_H
#define PIPELINE_GENERIC_FILTER_H


#include "filter.h"
#include "pipeline.h"
#include "function_filter.h"
#include "as_pipeline.h"

namespace pipeline {
    template <class T>
    class generic_filter final : public filter<T> {
    private:
        class in_source final : public source_binding<T> {
            generic_filter<T> *_filter;
            std::function<void(T)> push;

        public:
            explicit in_source(generic_filter<T> *_filter) : _filter(_filter) {}

            void bind(std::function<void(T)> push, std::function<void()> unbind) override {
                this->push = push;
            }

            void accept(T value) {
                push(value);
            }

            virtual ~in_source() {}
        };

        std::unique_ptr<filter<T>> _filter;
        in_source *_source;

        void bind_internal() {
            this->_filter->bind(_source);
            *this->_filter | [this] (T value) { this->push(value); };
        }

        explicit generic_filter(std::unique_ptr<filter<T>> _filter) : _filter(std::move(_filter)), _source(new in_source(this)) {
            bind_internal();
        }

        explicit generic_filter(filter<T> *_filter) : generic_filter(std::unique_ptr<filter<T>>(_filter)) {}

        template <class TFilter>
        static filter<T> *get_pointer(const TFilter &_filter) {
            return dynamic_cast<filter<T> *>(new TFilter(_filter));
        }

    protected:
        void accept(T value) override {
            _source->accept(value);
        }

    public:
        template <class TFilter>
        generic_filter(const TFilter &_filter) : generic_filter(get_pointer(as_pipeline<T, TFilter>::get(_filter))) {} // NOLINT

        generic_filter(generic_filter<T> &&other) noexcept : filter<T>(std::move(other)), _filter(std::move(other._filter)), _source(new in_source(this)) {
            _filter->unbind();
            _filter->unbind_all();
            bind_internal();
        }
    };
}


#endif //PIPELINE_GENERIC_FILTER_H
