/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <f5/makham/executor.hpp>

#include <experimental/coroutine>
#include <future>


namespace f5::makham {


    template<typename R>
    class future final {
      public:
        ~future() {
            if (coro) coro.destroy();
        }

        /// Not copyable
        future(future const &) = delete;
        future &operator=(future const &) = delete;
        /// Movable
        future(future &&t) noexcept : coro(t.coro) { t.coro = {}; }
        future &operator=(future &&t) noexcept { swap(coro, t.coro); }

        struct promise_type {
            using handle_type =
                    std::experimental::coroutine_handle<promise_type>;

            std::promise<R> fp = {};

            auto get_return_object() {
                return future{handle_type::from_promise(*this)};
            }
            auto return_value(R v) {
                fp.set_value(std::move(v));
                return std::experimental::suspend_never{};
            }
            void unhandled_exception() {
                fp.set_exception(std::current_exception());
            }

            auto initial_suspend() {
                return std::experimental::suspend_always{};
            }
            auto final_suspend() { return std::experimental::suspend_always{}; }
        };

        R get() { return coro.promise().fp.get_future().get(); }

      private:
        friend promise_type;
        typename promise_type::handle_type coro;

        future(typename promise_type::handle_type h) : coro(h) {
            f5::makham::post([this]() {
                std::cout << "Starting future " << &coro.promise() << std::endl;
                coro.resume();
            });
        }
    };


}
