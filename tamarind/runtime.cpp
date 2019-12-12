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
    f5::threading::tsmap<f5::u8string, makham::task<workflow>> workflows;
    std::vector<std::experimental::coroutine_handle<>> resumables;
};


f5::tamarind::runtime::runtime() : pimpl{std::make_unique<impl>()} {}
f5::tamarind::runtime::~runtime() = default;


std::future<int> f5::tamarind::runtime::completion() {
    return status_code.get_future();
}


namespace {
    f5::makham::task<f5::tamarind::workflow> wf(fostlib::json ast) {
        co_return f5::tamarind::workflow(ast);
    }
}


void f5::tamarind::runtime::load(fostlib::fs::path fn) {
    f5::makham::post([this, fn = std::move(fn)]() {
        auto [name, ast] = f5::tamarind::parse::workflow(fn);
        pimpl->workflows.add_if_not_found(name, [ast = std::move(ast)]() {
            auto t = wf(std::move(ast));
            return t;
        });
    });
}
