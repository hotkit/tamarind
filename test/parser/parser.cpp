/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <f5/tamarind/parse.hpp>

#include <fost/cli>
#include <fost/main.hpp>


namespace {
    const fostlib::setting<fostlib::nullable<fostlib::string>> c_save_filename(
            __FILE__,
            "tamarind-test-parser",
            "Save JSON to",
            fostlib::null,
            true);

    bool load_and_check(
            fostlib::ostream &out,
            fostlib::json data,
            fostlib::fs::path const &filename) {
        auto const expected =
                fostlib::utf::load_file(filename, fostlib::string{});
        fostlib::utf::save_file(filename, fostlib::json::unparse(data, true));
        if (expected.empty()) {
            out << "Expectation file not found. Saved new version" << std::endl;
            return true;
        }
        bool const correct = (fostlib::json::parse(expected) == data);
        if (not correct) {
            out << "Expected: " << expected << std::endl;
            out << "Got: " << data << std::endl;
        }
        return correct;
    }
}


FSL_MAIN("tamarind-test-parser", "Tamarind test parser")
(fostlib::ostream &out, fostlib::arguments &args) {
    args.commandSwitch("o", c_save_filename);

    auto const parsed = f5::tamarind::parse::workflow(
            fostlib::coerce<fostlib::fs::path>(args[1].value()));

    if (c_save_filename.value()) {
        if (not load_and_check(
                    out, parsed,
                    fostlib::coerce<fostlib::fs::path>(
                            c_save_filename.value()))) {
            return 1;
        }
    }

    return 0;
}
