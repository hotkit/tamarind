/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <f5/makham/executor.hpp>

#include <experimental/coroutine>
#include <future>
#include <iostream>
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
            std::atomic<bool> started = false;
            std::atomic<std::experimental::coroutine_handle<>> cb;
            std::promise<R> fp;

            void signal(std::experimental::coroutine_handle<> s) {
                auto old = cb.exchange(s);
                if (old) {
                    throw std::invalid_argument{
                            "A task can only have one awaitable"};
                }
            }

            auto get_return_object() {
                return task<R>{std::experimental::coroutine_handle<
                        promise_type>::from_promise(*this)};
            }
            auto initial_suspend() {
                return std::experimental::suspend_always{};
            }
            auto return_value(R v) {
                fp.set_value(v);
                value = std::move(v);
                return std::experimental::suspend_never{};
            }
            auto final_suspend() { return std::experimental::suspend_always{}; }
            void unhandled_exception() {
                std::cerr << "unhandled_exception in a task\n";
            }
        };

        std::experimental::coroutine_handle<> handle() const { return coro; }

        void start_async() {
            auto const running = coro.promise().started.exchange(true);
            if (not running) {
                f5::makham::post([this]() { coro.resume(); });
            }
        }

        auto as_future() {
            start_async();
            return coro.promise().fp.get_future();
        }

        /// ### Awaitable
        bool await_ready() const { return false; }
        void await_suspend(std::experimental::coroutine_handle<> awaiting) {
            coro.promise().signal(awaiting);
            start_async();
        }
        auto await_resume() { return *coro.promise().value; }

      private:
        using handle_type = std::experimental::coroutine_handle<promise_type>;
        handle_type coro;

        task(handle_type c) : coro{c} {}
    };


}
