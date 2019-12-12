/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <f5/tamarind/parse.hpp>
#include <f5/tamarind/runtime.hpp>
#include <f5/makham/executor.hpp>

#include <f5/threading/map.hpp>

#include <iostream>


struct f5::tamarind::runtime::impl {
    /// Store the workflows
    f5::threading::tsmap<f5::u8string, makham::task<workflow>> workflows;
    std::vector<std::experimental::coroutine_handle<>> resumables;
};


f5::tamarind::runtime::runtime() : self{std::make_unique<impl>()} {}
f5::tamarind::runtime::~runtime() = default;


namespace {
    f5::makham::task<f5::tamarind::workflow> wfexec(fostlib::json ast) {
        co_return f5::tamarind::workflow(ast);
    }
}


f5::makham::task<f5::u8string>
        f5::tamarind::runtime::load(fostlib::fs::path fn) {
    auto [name, ast] = f5::tamarind::parse::workflow(fn);
    co_return name;
}
