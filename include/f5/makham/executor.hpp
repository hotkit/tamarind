/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <thread-pool/fixed_function.hpp>


namespace f5::makham {


    /// Fixes size for the closure allowed when posting functions
    /// to the executor.
    using function_type = tp::FixedFunction<void(), 128>;


    /// Execute the function in the Makham executor's thread pool.
    void post(function_type);


}
