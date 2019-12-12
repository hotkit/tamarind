/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <fost/core>
#include <fost/exception/parse_error.hpp>


namespace f5::tamarind {


    /// A workflow loaded from a script
    class workflow {
      public:
        workflow(fostlib::json ast);
    };


}
