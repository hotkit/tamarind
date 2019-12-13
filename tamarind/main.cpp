#include <fost/cli>
#include <fost/main.hpp>

#include <f5/makham/unit.hpp>
#include <f5/tamarind/runtime.hpp>


FSL_MAIN("tamarind", "Tamarind runtime")
(fostlib::ostream &out, fostlib::arguments &args) {
    f5::tamarind::runtime rt;
    f5::makham::unit<f5::u8string> loaders;
    for (auto const &arg : args) {
        loaders.add(rt.load(fostlib::coerce<fostlib::fs::path>(arg)));
    }
    out << "Waiting for completion of work unit\n";
    loaders.block();
    return 0;
}
