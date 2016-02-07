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


        /// Merge two streams of the same underlying value together
        template<typename V>
        auto merge(output<V> s1, output<V> s2) {
            output<V> into;
            auto cb = [t1 = s1.s, t2 = s2.s](auto &s, auto v) {
                s.push(v);
            };
            s1.s->template on_value<V>(into.s, cb);
            s2.s->template on_value<V>(into.s, cb);
            return into;
        }


    }


}

