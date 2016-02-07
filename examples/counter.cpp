/*
    Copyright 2016, Felspar Co Ltd. http://www.kirit.com/f5
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/
// Compile with:
//      clang++ -std=c++14 -I../include counter.cpp -o counter


#include <iostream>
#include <f5/tamarind/frp.hpp>


struct counter {
    f5::input<int> increment;
    f5::input<int> decrement;
    f5::output<int> count = f5::merge(
        f5::on(increment, [this](int amount) { return count.value(0) + amount; }),
        f5::on(decrement, [this](int amount) { return count.value(0) - amount; }));
};


int main() {
    counter c1;
    // Add an observer to the count value
    c1.count.on_value(
        [](auto a) {
            std::cout << "Count now " << a << std::endl;
        });

    c1.increment.push(2);
    {
        // This observer's lifetime is scope limited
        auto short_lived_observer = on(c1.count,
            [](auto c) {
                std::cout << "Short lived observer: " << c << std::endl;
                return c;
            });
        c1.decrement.push(10);
    }

    c1.increment.push(3);
    std::cout << "final " << c1.count.value() << std::endl;

    return 0;
}

