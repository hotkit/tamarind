/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <f5/tamarind/parse.hpp>
#include <f5/tamarind/runtime.hpp>

#include <thread-pool/thread_pool.hpp>

#include <iostream>


struct f5::tamarind::runtime::impl {
    tp::ThreadPool threads;
};


f5::tamarind::runtime::runtime() : pimpl{std::make_unique<impl>()} {}

f5::tamarind::runtime::~runtime() = default;


std::future<int> f5::tamarind::runtime::completion() {
    return status_code.get_future();
}


void f5::tamarind::runtime::load(fostlib::fs::path fn) {
    pimpl->threads.post([this, fn = std::move(fn)]() {
        try {
            auto const wf = parse::workflow(fn);
            status_code.set_value(0);
        } catch (fostlib::exceptions::exception &e) {
            std::cerr << e << std::endl;
            status_code.set_value(1);
        } catch (...) { status_code.set_value(127); }
    });
}
