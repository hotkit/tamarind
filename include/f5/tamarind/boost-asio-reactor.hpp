/*
    Copyright 2016, Felspar Co Ltd. http://www.kirit.com/f5
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#pragma once


#include <f5/threading/boost-asio.hpp>
#include <f5/tamarind/async.hpp>
#include <f5/tamarind/on.hpp>

#include <boost/asio/strand.hpp>


namespace f5 {


    namespace boost_asio {


        /// A generic I/O block whose internal wiring runs inside a
        /// strand in the reactor.
        class bridge {
            reactor_pool &reactor;

        public:
            /// The thread barrier needed to enter the components
            boost::asio::io_service::strand barrier;

            /// Construct a bridge and barrier for this reactor pool
            explicit bridge(reactor_pool &rp)
            : reactor(rp), barrier(rp.get_io_service()) {
            }

            /// General data structure that crosses the barrier
            template<typename P>
            class jumper;

            /// Specialisation for inputs
            template<typename V>
            using input = jumper<tamarind::input<V>>;
        };


        /// An input boudary for the block
        template<typename V>
        class bridge::jumper<tamarind::input<V>> {
            bridge &block;
            tamarind::input<V> &stream;
        public:
            jumper(bridge &b, tamarind::input<V> &s)
            : block(b), stream(s) {
            }

            void push(V v) {
                block.barrier.post([s = stream.s, v = std::move(v)]() {
                    s->push(v);
                });
            }
        };


    }


}

