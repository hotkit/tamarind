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


        /// Combine the values on two streams into a single value
        template<typename V, typename A1, typename A2, typename F>
        output<V> combine(output<A1> &b1, output<A2> &b2, F lambda) {
            output<V> combined;
            auto wrapped =
                // TODO capture weak pointers
                [lambda, b1s = b1.s, b2s = b2.s]
                (detail::stream<V> &stream, auto) {
                    if ( b1s->has_value() && b2s->has_value() ) {
                        stream.push(lambda(b1s->value(), b2s->value()));
                    }
                };
            b1.s->template on_value<V>(combined.s, wrapped);
            b2.s->template on_value<V>(combined.s, wrapped);
            return combined;
        }


    }


}

