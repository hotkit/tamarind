/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <f5/makham/executor.hpp>

#include <thread-pool/thread_pool.hpp>


namespace {
    tp::ThreadPool threads;
}


void f5::makham::post(function_type f) {
    threads.post(std::move(f));
}
