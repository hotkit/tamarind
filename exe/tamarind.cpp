#include <fost/cli>
#include <fost/main.hpp>

#include <f5/tamarind/runtime.hpp>


FSL_MAIN("tamarind", "Tamarind runtime")
(fostlib::ostream &out, fostlib::arguments &args) {
    f5::tamarind::runtime rt;
    for (auto const &arg : args) {
        out << arg << std::endl;
        rt.load(fostlib::coerce<fostlib::fs::path>(arg));
    }
    return rt.completion().get();
}
