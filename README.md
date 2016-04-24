# Tamarind #

Tamarind is a functional reactive programming implementation (FRP) for modern C++. It is designed for use in a number of different environments including normal C++, Boost ASIO and [Emscripten](emscripten.md).

Tamarind is a header only library that requires a C++14 compiler. Initial versions of the library target push only streams. Once coroutines become available then Tamarind will implement pull streams using them. The new "STL v2" is also relevant for pull streams.


## Obligatory "counter" example ##


    #include <iostream>
    #include <f5/tamarind/frp.hpp>


    struct counter {
        f5::input<int> increment;
        f5::input<int> decrement;
        f5::output<int> count = f5::merge(
            f5::on(increment, [this](int amount) { return count.value(0) + amount; }),
            f5::on(decrement, [this](int amount) { return count.value(0) - amount; }));
    };


    void main() {
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
                    std::cout << "Short lived observer: " << c1.count.value() << std::endl;
                });
            c1.decrement.push(10);
        }

        c1.increment.push(3);
        std::cout << "final " << c1.count.value() << std::endl;
    }

Running this will show the following:

    Count now 2
    Short lived observer: 2
    Count now -8
    Short lived observer: -8
    Count now -5
    final -5

Note that the short lived observer is called straight away when it is attached to the count stream because the stream already has a value. Details of the APIs can be found in the [reference](reference.md).


## Concepts ##

Tamarind defines a number of port types that can be used to wire together the data flow and processing. The simplest are input and output ports. Output ports can have their values read and observers added to them to respond to signals when they arrive. Every port in Tamarind is an output port. Input ports also allow you to push values into them.

There are a number of functions that are used to build streams out of other streams, these include `on`, `merge` and `combine`.


### Thread safety ###

There are no special provisions for thread safety in Tamarind. Streams are not themselves thread safe and care must be taken not to push values into streams from different threads at any given time.

The `ioblock` can be used for processing through a Boost ASIO reactor pool. This makes use of the `f5::boost_asio::reactor_pool` and a strand for each block. A block is a part of the FRP graph that runs in its own strand in the reactor.

The inputs  can be wired from other parts of the program and used from multiple threads at once. The valuess are posted into the strand and the internal wiring of the block runs inside that strand. The internal wiring must be wired in through use of the `iobloc::on` method rather than the global one.



