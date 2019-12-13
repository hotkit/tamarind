/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <f5/makham/task.hpp>

#include <vector>


namespace f5::makham {


    /// ## Work unit
    /**
     */
    template<typename R>
    class unit {
        std::vector<task<R>> resumables;

      public:
        void add(task<R> r) { resumables.push_back(std::move(r)); }
        std::size_t block();
    };


}


namespace std::experimental {
    template<typename R>
    struct coroutine_traits<std::future<void>, f5::makham::unit<R> &> {
        struct promise_type {
            std::promise<void> donep;

            auto get_return_object() { return donep.get_future(); }
            auto initial_suspend() {
                return std::experimental::suspend_always{};
            }
            auto return_void() {
                donep.set_value();
                return std::experimental::suspend_never{};
            }
            auto final_suspend() { return std::experimental::suspend_always{}; }
            void unhandled_exception() {
                std::cerr << "unhandled_exception for std::future<> wrapper\n";
            }
        };
    };
}


template<typename R>
inline std::size_t f5::makham::unit<R>::block() {
    auto t = [this]() -> task<std::size_t> {
        /// Start every resumable as separate tasks
        for (auto &t : resumables) { t.start_async(); }
        /// Wait for them all to finish
        std::size_t count{};
        for (auto &t : resumables) {
            co_await t;
            ++count;
        }
        co_return count;
    }();
    auto f = t.as_future();
    return f.get();
}
