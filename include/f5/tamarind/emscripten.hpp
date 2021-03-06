/*
    Copyright 2016, Felspar Co Ltd. http://www.kirit.com/f5
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <string>
#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <f5/tamarind/async.hpp>
#include <f5/tamarind/frp.hpp>


namespace f5 {


    inline namespace tamarind {


        namespace emscripten {


            /// Expose the FRP streams for a specified type
            template<typename V> inline
            void frp() {
                using namespace ::emscripten;
                class_<output<V>>(typeid(output<V>).name())
                        .function("onValue", (void (*)(output<V> *, ::emscripten::val))
                            ([](auto self, ::emscripten::val fun) -> void {
                                self->on_value([fun](const V &v) mutable {
                                    fun(::emscripten::val(v));
                                });
                            }), ::emscripten::allow_raw_pointers())
                        .function("value", select_overload<const V&(void) const>(&output<V>::value))
                    ;
                class_<input<V>, base<output<V>>>(typeid(input<V>).name())
                        .function("push", &input<V>::push)
                    ;
                class_<async<V>, base<input<V>>>(typeid(async<V>).name())
                        .function("onStart", (void (*)(async<V> *, ::emscripten::val))
                            ([](auto self, ::emscripten::val fun) -> void {
                                self->on_start([fun]() mutable {
                                    fun();
                                });
                            }), ::emscripten::allow_raw_pointers())
                    ;
            }


        }


    }


}

