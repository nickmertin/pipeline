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
    class source {
    private:
        class true_source_binding final : public source_binding<T> {
        private:
            source<T> *_source;
            std::function<void(T)> _push;
            std::function<void()> _unbind;

        public:
            true_source_binding(source<T> *_source) : _source(_source) {}

            void bind(std::function<void(T)> push, std::function<void()> unbind) override {
                _push = push;
                _unbind = unbind;
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

        std::unordered_set<true_source_binding *> bindings;

    protected:
        void push(T value) {
            for (true_source_binding *b : bindings) {
                b->push(value);
            }
        }

    public:
        template<class TSink>
        TSink &operator|(TSink &other) {
            dynamic_cast<sink<T> *>(&other)->bind(new true_source_binding(this));
            return other;
        }

        virtual ~source() {
            for (true_source_binding *b : bindings) {
                b->unbind();
            }
        }
    };
}


#endif //PIPELINE_SOURCE_H
