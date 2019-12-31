/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <f5/tamarind/workflow.hpp>

#include <iostream>


f5::tamarind::workflow::workflow(fostlib::json ast) {
    std::cout << "Loaded " << ast[1] << std::flush;
}
