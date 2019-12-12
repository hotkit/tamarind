/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <f5/tamarind/parse.hpp>
#include <f5/tamarind/runtime.hpp>
#include <f5/makham/executor.hpp>

#include <f5/threading/map.hpp>


struct f5::tamarind::runtime::impl {
    /// Store the workflows
    f5::threading::tsmap<f5::u8shared, makham::task<workflow>> workflows;
};


f5::tamarind::runtime::runtime() : pimpl{std::make_unique<impl>()} {}
f5::tamarind::runtime::~runtime() = default;


std::future<int> f5::tamarind::runtime::completion() {
    return status_code.get_future();
}


namespace {
    f5::makham::task<f5::tamarind::workflow> wf(fostlib::fs::path fn) {
        co_return f5::tamarind::workflow(f5::tamarind::parse::workflow(fn));
    }
}


void f5::tamarind::runtime::load(fostlib::fs::path fn) {
    f5::makham::post([this, fn = std::move(fn)]() {
    });
}
