/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <f5/makham/async.hpp>
#include <f5/makham/future.hpp>

#include <vector>


namespace f5::makham {


    /// ## Work unit
    /**
     */
    template<typename R>
    class unit {
        std::vector<async<R>> resumables;

      public:
        void add(async<R> r) { resumables.push_back(std::move(r)); }
        std::size_t block();
    };


}


template<typename R>
inline std::size_t f5::makham::unit<R>::block() {
    auto const t = [this]() -> async<std::size_t> {
        std::cout << "Starting to resume " << resumables.size() << " coroutines"
                  << std::endl;
        /// Start every resumable as separate tasks
        for (auto &t : resumables) { t.start(); }
        /// Wait for them all to finish
        std::size_t count{};
        for (auto &t : resumables) {
            std::cout << "Awating async" << std::endl;
            co_await t;
            ++count;
        }
        co_return count;
    };
    auto const f = [this](async<std::size_t> a) -> future<std::size_t> {
        std::cout << "Waiting for async" << std::endl;
        a.start();
        co_return co_await a;
    };
    return f(t()).get();
}
