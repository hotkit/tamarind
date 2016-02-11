# Tamarind and Emscripten #

You'll need to include the `emscripten.hpp` header file:

    #include <f5/tamarind/emscripten.hpp>

And you will also need to include an `EMSCRIPTEN_BINDINGS` block to expose the relevant types. There is a utility function `f5::tamarind::emscripten::frp` that can be used to expose the stream types you're using to JavaScript.


    EMSCRIPTEN_BINDINGS(chronon) {
        f5::emscripten::frp<bool>("bool");
        f5::emscripten::frp<int>("int");
    }


Becuase the environment is asynchronous by nature the `f5::tamarind::async` stream type is included by default.
