/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <f5/makham/async.hpp>
#include <f5/tamarind/workflow.hpp>

#include <fost/core>

#include <future>


namespace f5::tamarind {


    /// # Language runtime
    /**
     */
    class runtime {
      public:
        struct impl;

      private:
        std::unique_ptr<impl> self;

      public:
        runtime();
        ~runtime();

        /// ## Script loading and starts
        makham::async<void> load(fostlib::fs::path);
    };


}
