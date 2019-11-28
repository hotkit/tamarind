/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <f5/tamarind/node.hpp>

#include <fost/main>


namespace {


    struct counter {
        constexpr counter() noexcept : current{} {}

        void send(int change) { current += change; }

      private:
        int current;
    };


}


FSL_MAIN("counter", "Tamarind counter examples")
(fostlib::ostream &out, fostlib::arguments &) {
    counter c1;
    return 0;
}
