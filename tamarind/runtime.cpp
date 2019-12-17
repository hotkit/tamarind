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
    f5::threading::tsmap<f5::u8string, workflow> workflows;
};


f5::tamarind::runtime::runtime() : self{std::make_unique<impl>()} {}
f5::tamarind::runtime::~runtime() = default;


f5::makham::async<void> f5::tamarind::runtime::load(fostlib::fs::path fn) {
    std::cout << fn << std::endl;
    auto [name, ast] = f5::tamarind::parse::workflow(fn);
    self->workflows.insert_or_assign(
            name, f5::tamarind::workflow(std::move(ast)));
    co_return;
}
