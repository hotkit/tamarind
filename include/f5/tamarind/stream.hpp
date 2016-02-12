/*
    Copyright 2016, Felspar Co Ltd. http://www.kirit.com/f5
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
#include <vector>


namespace f5 {


    inline namespace tamarind {


        namespace detail {


            /// Internal implementation of the streams
            template<typename V>
            class stream {
                std::function<bool(V)> predicate;
                std::vector<std::function<bool(V)>> callbacks;
                std::unique_ptr<V> last;

                /// When we hold a callback we wrap it so we can determine
                /// if we still need the callback.
                void callback(V v) {
                    std::remove_if(callbacks.begin(), callbacks.end(),
                        [&v](auto cb) {
                            return cb(v);
                        });
                }
            public:
                /// Pass a lambda that returns true if we want the value. All predicates
                /// must pass if more than one is registered.
                template<typename F>
                void filter(F pred) {
                    if ( predicate ) {
                        predicate = [pred, old = predicate](V v) {
                                return old(v) && pred(v);
                            };
                    } else {
                        predicate = pred;
                    }
                }

                void push(V v) {
                    if ( predicate && not predicate(v) ) {
                        return;
                    }
                    if ( not last ) {
                        last = std::make_unique<V>(v);
                    } else {
                        *last = v;
                    }
                    callback(*last);
                }

                template<typename Y>
                void on_value(
                    std::shared_ptr<stream<Y>> into, std::function<void(stream<Y> &, V)> cb
                ) {
                    if ( last ) {
                        cb(*into, *last);
                    }
                    /// The callback has to be wrapped and hold the target
                    /// stream reference weakly. This means that if the target
                    /// goes out of scope then the callback won't fire and
                    /// it can be cleared out of this stream's callback list.
                    callbacks.push_back([sink = std::weak_ptr<stream<Y>>(into), cb](V v) {
                            std::shared_ptr<stream<Y>> ptr(sink.lock());
                            if ( ptr ) {
                                cb(*ptr, v);
                                return false;
                            } else {
                                return true;
                            }
                        });
                }

                template<typename Y>
                void when_value(std::shared_ptr<stream<Y>> p, std::function<void(void)> cb) {
                    this->template on_value<Y>(p, [cb](auto &, auto) { cb(); });
                }

                bool has_value() const {
                    return last ? true : false;
                }

                V value(V def) {
                    if ( last ) {
                        return *last;
                    } else {
                        return def;
                    }
                }
                V value() {
                    if ( last ) {
                        return *last;
                    } else {
                        throw std::invalid_argument("There is no value in the stream");
                    }
                }
            };


            template<typename V>
            struct stream_wrapper {
                std::shared_ptr<stream<V>> s;
                stream_wrapper()
                : s(new stream<V>) {
                }
            };


        }


        /// Output port
        template<typename V>
        struct output {
            std::shared_ptr<detail::stream<V>> s;

            /// Construct an output port
            output()
            : s(new detail::stream<V>) {
            }
            /// Construct from an API building object
            output(detail::stream_wrapper<V> sw)
            : s(sw.s) {
            }

            void on_value(std::function<void(V)> cb) {
                s->template on_value<V>(s, [cb](auto &s, auto v) {
                        cb(v);
                    });
            }
            void when_value(std::function<void(void)> cb) {
                s->template when_value<V>(s, cb);
            }

            V value(V def) {
                return s->value(def);
            }
            V value() {
                return s->value();
            }
        };


        /// An input port (also includes the output port)
        template<typename V>
        struct input : public output<V> {
            void push(V v) {
                this->s->push(v);
            }
        };


        namespace detail {


            /// Used to implement parts of the DSL
            template<typename V>
            struct partial : stream_wrapper<V> {
                /// Use a bool output to gate the setting of a value in this stream
                partial &when(output<bool> &b) {
                    // TODO Close around a weak_ptr
                    this->s->filter([condition = b.s](auto) {
                            return condition->value(false);
                        });
                    return *this;
                }
                /// Use a bool output to gate the setting of a value in this stream
                partial &when_not(output<bool> &b) {
                    // TODO Close around a weak_ptr
                    this->s->filter([condition = b.s](auto) {
                            return not condition->value(true);
                        });
                    return *this;
                }

                /// Set up to feed this output into the specified feed
                template<typename T>
                auto feeds(input<T> &b) {
                    return std::make_pair(*this, b);
                }
            };


        }


    }


}

