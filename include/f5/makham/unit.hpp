/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <f5/makham/executor.hpp>
#include <f5/makham/task.hpp>

#include <future>
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
        std::future<void> block();
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
                return std::experimental::suspend_always{};
            }
            auto final_suspend() { return std::experimental::suspend_always{}; }
            void unhandled_exception() { std::exit(116); }
        };
    };
}


template<typename R>
inline std::future<void> f5::makham::unit<R>::block() {
    /// Start every resumable as separate tasks
    for (auto &t : resumables) {
        f5::makham::post([h = t.handle()]() mutable { h.resume(); });
    }
    /// Wait for them all to finish
    for (auto &t : resumables) { co_await t; }
}
