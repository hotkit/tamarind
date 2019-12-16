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
#include <variant>


namespace f5 {
    namespace detail {
        template<typename... Fs>
        struct visitor_overload : std::remove_reference_t<Fs>... {
            visitor_overload(Fs &&... fs)
            : std::remove_reference_t<Fs>{std::forward<Fs>(fs)}... {}
            using std::remove_reference_t<Fs>::operator()...;
        };
    }
    template<typename V, typename... T>
    decltype(auto) apply_visitor(V &&v, T &&... t) {
        return std::visit(
                detail::visitor_overload<T...>(std::forward<T>(t)...),
                std::forward<V>(v));
    }
}


namespace f5::makham {


    /// Forward declarations
    template<typename R>
    struct promise_type;
    template<typename R, typename P = promise_type<R>>
    class task;


    /// ## Task
    /**
     * A task whose completion can be awaited..
     */
    template<typename R, typename P>
    class task final {
      public:
        using promise_type = P;
        friend promise_type;

        ~task() {
            if (coro) coro.destroy();
        }

        /// Not copyable
        task(task const &) = delete;
        task &operator=(task const &) = delete;
        /// Movable
        task(task &&t) noexcept : coro(t.coro) { t.coro = {}; }
        task &operator=(task &&t) noexcept { swap(coro, t.coro); }

        std::experimental::coroutine_handle<> handle() const { return coro; }

        /// This can be called from many places and it is vital that we only
        /// start exactly once.
        void start_async() {
            auto const running = coro.promise().started.exchange(true);
            if (not running) {
                f5::makham::post([this]() {
                    std::cout << "Starting " << &coro.promise() << std::endl;
                    coro.resume();
                });
            }
        }
        auto as_future() {
            start_async();
            return coro.promise().fp.get_future();
        }

        /// ### Awaitable
        bool await_ready() const { return false; }
        void await_suspend(std::experimental::coroutine_handle<> awaiting) {
            start_async();
            coro.promise().signal(awaiting);
        }
        R await_resume() {
            auto &p = coro.promise();
            return apply_visitor(
                    std::move(p.value),
                    [](std::monostate) -> R {
                        throw std::runtime_error(
                                "The coroutine doesn't have a value");
                    },
                    [](std::exception_ptr e) -> R { std::rethrow_exception(e); },
                    [](R v) { return v; });
        }

      private:
        using handle_type = std::experimental::coroutine_handle<promise_type>;
        handle_type coro;

        task(handle_type c) : coro{c} {
            std::cout << "Created task " << &coro.promise() << std::endl;
        }
    };


    /// An asynchronous promise
    struct async_promise {
        std::atomic<bool> has_value = false;
        std::atomic<bool> started = false;
        std::atomic<std::experimental::coroutine_handle<>> continuation = {};

        void continuation_if_not_run() {
            auto h = continuation.exchange({});
            if (h) {
                std::cout << "Continuation for " << this << std::endl;
                h.resume();
            } else {
                std::cout << "No continuation to run for " << this << std::endl;
            }
        }
        void signal(std::experimental::coroutine_handle<> s) {
            auto const old = continuation.exchange(s);
            if (old) {
                throw std::invalid_argument{
                        "A task can only have one awaitable"};
            }
            auto const done = has_value.exchange(false);
            if (done) {
                std::cout << "Value arrived when setting signal" << std::endl;
                continuation_if_not_run();
            }
        }
        void value_has_been_set() {
            std::cout << "Value has been set" << std::endl;
            if (has_value.exchange(true)) {
                throw std::runtime_error{"Coroutine already had a value set"};
            }
            continuation_if_not_run();
        }
        auto initial_suspend() { return std::experimental::suspend_always{}; }
        auto final_suspend() { return std::experimental::suspend_always{}; }
    };


    /// ## The task promise type
    template<typename R>
    struct promise_type : public async_promise {
        using task_type = task<R, promise_type<R>>;
        using handle_type =
                std::experimental::coroutine_handle<promise_type<R>>;

        std::variant<std::monostate, std::exception_ptr, R> value = {};
        std::promise<R> fp = {};

        auto get_return_object() {
            return task_type{handle_type::from_promise(*this)};
        }
        auto return_value(R v) {
            fp.set_value(v);
            value = std::move(v);
            value_has_been_set();
            return std::experimental::suspend_never{};
        }
        void unhandled_exception() {
            value = std::current_exception();
            value_has_been_set();
        }
    };


}
