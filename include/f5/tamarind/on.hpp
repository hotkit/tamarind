/*
    Copyright 2016, Felspar Co Ltd. http://www.kirit.com/f5
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <f5/tamarind/stream.hpp>


namespace f5 {


    inline namespace tamarind {


        /// Duplciate a stream and then expose new APIs
        template<typename T>
        detail::partial<T> on(output<T> &trigger) {
            detail::partial<T> duplicate;
            trigger.s->template on_value<T>(duplicate.s, [](auto &s, auto v) {
                s.push(v);
            });
            return duplicate;
        }
        /// Duplicate a stream and perform some transformation on the values
        template<typename T, typename F>
        auto on(output<T> &trigger, F lambda) {
            typedef decltype(lambda(T{})) V;
            output<V> sink;
            trigger.s->template on_value<V>(sink.s,
                [lambda](auto &s, auto v) {
                    s.push(lambda(v));
                });
            return sink;
        }


    }


}

