/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <experimental/coroutine>
#include <optional>


namespace f5::makham {


    /// ## Task
    /**
     * A task whose completion can be awaited..
     */
    template<typename R>
    class task {
      public:
        struct promise_type {
            std::optional<R> value;

            auto get_return_object() { return task<R>{}; }
            auto initial_suspend() {
                return std::experimental::suspend_always{};
            }
            auto return_value(R v) {
                value = std::move(v);
                return std::experimental::suspend_always{};
            }
            auto final_suspend() { return std::experimental::suspend_always{}; }
            void unhandled_exception() { std::exit(117); }
        };

      private:
        using handle_type = std::experimental::coroutine_handle<promise_type>;
        handle_type coro;
    };


}
