/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <experimental/coroutine>
#include <future>


namespace f5::makham {


    /// ## Task
    /**
     * A task whose completion can be awaited..
     */
    template<typename R>
    class task {
        std::future<R> future;

      public:
        struct promise_type {
            std::promise<R> value;

            auto get_return_object() { return task<R>{value.get_future()}; }
            auto initial_suspend() {
                return std::experimental::suspend_always{};
            }
            auto return_value(R v) {
                value.set_value(std::move(v));
                return std::experimental::suspend_always{};
            }
            auto final_suspend() { return std::experimental::suspend_always{}; }
            void unhandled_exception() { std::exit(117); }
        };

        std::experimental::coroutine_handle<> handle() const { return coro; }


        /// ### Awaitable
        bool await_ready() const { return false; }

        void await_suspend(std::experimental::coroutine_handle<> awaiting) {}

        auto await_resume() { return future.get(); }

      private:
        friend struct promise_type;

        task(std::future<R> f) : future(std::move(f)) {}

        using handle_type = std::experimental::coroutine_handle<promise_type>;
        handle_type coro;
    };


}
