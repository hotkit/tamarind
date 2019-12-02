/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <fost/core>


namespace f5::tamarind::parse {


    /// Parse a workflow file
    fostlib::json workflow(fostlib::fs::path const &filename);


}
