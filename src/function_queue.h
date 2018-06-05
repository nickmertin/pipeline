//
// Created by nmeritn on 2018-06-04.
//

#ifndef PIPELINE_FUNCTION_QUEUE_H
#define PIPELINE_FUNCTION_QUEUE_H


#include <functional>

namespace pipeline {
    template <class ...TParams>
    class function_queue final {
    public:
        typedef std::function<void(TParams...)> function_type;

    private:
        std::vector<function_type> queue;
        bool locked = false;
        std::tuple<TParams...> params;

        template <size_t ...I>
        void run_internal(const std::function<void(TParams...)> &f, std::index_sequence<I...>) {
            f(std::get<I>(params)...);
        }

        void run_internal(const std::function<void(TParams...)> &f) {
            run_internal(f, std::make_index_sequence<sizeof...(TParams)>());
        }

    public:
        explicit function_queue(const TParams &...params) : params(params...) {}

        void lock() {
            locked = true;
        }

        void unlock() {
            locked = false;
            for (auto &&f : queue) {
                run_internal(f);
            }
            queue.clear();
        }

        void run(const function_type &f) {
            if (locked)
                queue.push_back(f);
            else
                run_internal(f);
        }
    };
}


#endif //PIPELINE_FUNCTION_QUEUE_H
