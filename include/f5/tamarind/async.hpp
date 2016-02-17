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


        template<typename T>
        struct async : public input<T> {
            template<typename V>
            async(detail::partial<V> &t) {
                link(t.s);
            }
            template<typename V>
            async(std::pair<detail::partial<V>, input<T>> &&p) {
                link(p.first.s);
                this->s->template on_value<T>(p.second.s, [](auto &s, const T &v) {
                        s.push(v);
                    });
            }

            void on_start(std::function<void(void)> fun) {
                trigger.when_value([fun]() { fun(); });
            }

        private:
            // TODO should be input<void>
            input<bool> trigger;

            template<typename V>
            void link(std::shared_ptr<detail::stream<V>> s) {
                s->when_value(s, [this, s]() {
                        trigger.push(true);
                    });
            }
        };


    }


}

