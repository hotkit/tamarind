/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#include <f5/tamarind/parse.hpp>
#include <f5/tamarind/parser/workflow.hpp>

#include <fost/file>
#include <fost/insert>
#include <fost/push_back>


fostlib::json f5::tamarind::parse::workflow(fostlib::fs::path const &filename) {
    fostlib::json ret, into;
    fostlib::push_back(
            ret,
            fostlib::coerce<fostlib::string>(filename.extension()).substr(1));
    fostlib::push_back(ret, fostlib::coerce<fostlib::string>(filename.stem()));
    auto script = fostlib::utf::load_file(filename);
    auto pos = f5::cord::make_u32u16_iterator(script.begin(), script.end());
    f5::tamarind::parsers::workflow<decltype(pos.first)> rule;
    if (boost::spirit::qi::parse(pos.first, pos.second, rule, into)
        && pos.first == pos.second) {
        if (into.isobject()) {
            fostlib::push_back(ret, into);
        } else {
            for (auto &&sexpr : into) { fostlib::push_back(ret, sexpr); }
        }
        return ret;
    } else {
        f5::u8string parsed{script.begin(), pos.first.u32_iterator()};
        std::size_t line{1}, col{1};
        for (auto c : parsed) {
            if (c == '\n') {
                ++line;
                col = 1;
            } else {
                ++col;
            }
        }
        throw fostlib::exceptions::parse_error{"Unknown parse error", filename, line, col};
    }
    return ret;
}
