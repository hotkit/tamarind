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
        /// Not copyable
        task(task const &) = delete;
        task &operator=(task const &) = delete;
        /// Movable
        task(task &&t) : coro(t.coro) { t.coro = {}; }
        task &operator=(task &&t) { swap(coro, t.coro); }

        struct promise_type {
            std::exception_ptr exception = {};
            std::optional<R> value = {};
            std::atomic<bool> started = false;
            std::atomic<std::experimental::coroutine_handle<>> cb = {};
            std::promise<R> fp = {};

            void signal(std::experimental::coroutine_handle<> s) {
                std::cout << "Setting callback " << this << std::endl;
                auto old = cb.exchange(s);
                if (old) {
                    throw std::invalid_argument{
                            "A task can only have one awaitable"};
                }
                if (s && (value.has_value() || exception)) {
                    std::cout << "Executing cb because setting up signal"
                              << std::endl;
                    callback();
                }
            }
            void callback() {
                f5::makham::post([this]() {
                    std::cout << "Starting callback" << std::endl;
                    try {
                        auto h = cb.exchange({});
                        if (h) {
                            std::cout << "Callback from " << this << std::endl;
                            h.resume();
                        }
                    } catch (...) {
                        std::cerr << "Exception exited cb.resume()\n";
                    }
                });
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
                callback();
                return std::experimental::suspend_never{};
            }
            auto final_suspend() { return std::experimental::suspend_always{}; }
            void unhandled_exception() { exception = std::current_exception(); }
        };

        std::experimental::coroutine_handle<> handle() const { return coro; }

        void start_async() {
            auto const running = coro.promise().started.exchange(true);
            if (not running) {
                std::cout << "Starting async" << std::endl;
                f5::makham::post([this]() {
                    std::cout << "Resuming " << &coro.promise() << std::endl;
                    try {
                        coro.resume();
                    } catch (...) {
                        std::cerr << "Exception exited coro.resume()"
                                  << std::endl;
                    }
                });
            } else {
                std::cout << "Task already running" << std::endl;
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
        auto await_resume() {
            auto &p = coro.promise();
            if (p.exception) {
                std::rethrow_exception(p.exception);
            } else if (not p.value.has_value()) {
                throw std::runtime_error("The coroutine doesn't have a value");
            } else {
                return *coro.promise().value;
            }
        }

      private:
        using handle_type = std::experimental::coroutine_handle<promise_type>;
        handle_type coro;

        task(handle_type c) : coro{c} {}
    };


}
