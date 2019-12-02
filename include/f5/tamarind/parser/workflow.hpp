/**
    Copyright 2019 Red Anchor Trading Co. Ltd.

    Distributed under the Boost Software License, Version 1.0.
    See <http://www.boost.org/LICENSE_1_0.txt>
 */


#pragma once


#include <fost/core>
#include <fost/parse/json.hpp>
#include <fost/push_back>


namespace f5::tamarind::parsers {


    template<typename Iterator>
    struct workflow : boost::spirit::qi::grammar<Iterator, fostlib::json()> {
        using line_t = std::pair<std::optional<std::string>, fostlib::json>;
        using lines_t = std::vector<line_t>;
        boost::spirit::qi::rule<Iterator, fostlib::json()> top, block, atom,
                full_sexpr, auto_quote, scope;
        boost::spirit::qi::rule<Iterator, fostlib::json::array_t()> inner_sexpr;
        boost::spirit::qi::rule<Iterator, line_t()> line;
        boost::spirit::qi::rule<Iterator, lines_t()> lines;
        boost::spirit::qi::rule<Iterator, void()> space, newline, comment,
                linebreak;
        boost::spirit::qi::rule<Iterator, std::string()> comment_p, identifier;
        fostlib::json_embedded_parser<Iterator> json_p;
        std::unique_ptr<workflow> nested;

        workflow(std::size_t indent = 0u) : workflow::base_type(top) {
            namespace qi = boost::spirit::qi;

            space = qi::lit(' ');
            newline = qi::lit('\n');

            comment_p = qi::lit('#') >> *(qi::standard_wide::char_ - newline);
            comment = qi::omit[comment_p];

            identifier =
                    +(qi::standard_wide::char_
                      - (space | newline | ')' | ':' | '#' | '='));

            auto_quote =
                    (json_p.object | json_p.array
                     | json_p.json_string_p)[boost::phoenix::bind(
                            [](auto &v, auto j) {
                                v = boost::apply_visitor(
                                        [](auto p) {
                                            fostlib::json::array_t quote;
                                            quote.push_back(fostlib::json("q"));
                                            quote.push_back(fostlib::json(
                                                    std::move(p)));
                                            return quote;
                                        },
                                        j);
                            },
                            qi::_val, qi::_1)];

            atom = auto_quote | json_p.null | json_p.boolean | json_p.number
                    | full_sexpr | identifier | scope;
            inner_sexpr = atom % *space;
            full_sexpr = qi::lit('(') >> inner_sexpr >> qi::lit(')');
            line = -(identifier >> *space >> qi::lit('=') >> *space)
                    >> (auto_quote | scope | inner_sexpr);

            scope = qi::lit(':') >> qi::lazy(boost::phoenix::bind(
                            [indent, this]() -> workflow & {
                                if (not nested)
                                    nested = std::make_unique<workflow>(
                                            indent + 4u);
                                return *nested;
                            }));

            lines = line % +linebreak;
            block = lines[boost::phoenix::bind(
                    [](auto &v, auto &c) {
                        fostlib::json::object_t blk;
                        blk[fostlib::string{}] = fostlib::json::array_t{
                                {fostlib::json{"progn"}}};
                        bool use_object{false};
                        for (auto &&l : c) {
                            if (l.first) {
                                use_object = true;
                                blk[l.first.value()] = l.second;
                            } else {
                                fostlib::push_back(
                                        blk[fostlib::string()], l.second);
                            }
                        }
                        v = use_object ? fostlib::json(std::move(blk))
                                       : blk[fostlib::string{}];
                    },
                    qi::_val, qi::_1)];

            if (indent) {
                linebreak = *space >> -comment >> *newline
                        >> qi::omit[qi::string(std::string(indent, ' '))];
                top = qi::omit[*space >> *linebreak] >> block;
            } else {
                linebreak = *space >> -comment >> newline;
                top = qi::omit[*linebreak] >> -block >> qi::omit[*linebreak];
            }
        }
    };


}
