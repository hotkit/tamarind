# Tamarind API #

Everything is in the `f5::tamarind` namespace which is inlined into `f5`. There is a convenience header at `f5/tamarind` that can be used to access the API.


## Stream types ##


### `f5::tamarind::output<T>` ###

Output streams can have their values read and allow observation of when the value changes. Every Tamarind stream implements the `output` interface.

Output streams are created by using the `on` API to build a


#### `on_value` ####

Add an observer to a stream which is called with the new value. The observer cannot be removed from the stream.

    f5::output<int> count = ...
    count.on_value(
        [](int value) {
            std::cout << "Count is " << value << std::endl;
        });

This API is not available when the stream type is `void`.


#### `when_value` ####

Add an observer that is called when a value appears on the stream, but without passing in what the value is.


#### `value` ####

Returns the current value held on the stream.

    f5::output<int> count = ...
    // Return the current value, but throw if there isn't one yet
    count.value();
    // Return the current value, but use zero if there is no value yet
    count.value(0);


### `f5::tamarind::input<T>` ###

A stream that allows values to be placed on it and values to be read (because it is still also an output stream).


#### `push` ####

Sends a value into the stream and then updates all dependant streams.

    f5::output<int> s;
    s.push(5);
    s.push(10);


## Combining streams ##


### `on` ###

Captures the output of one stream and then allows building of further changes on top of its outputs. The resulting stream will be fed values so long as it is kept alive.

    f5::output<int> s = ....
    f5::output<int> s_squared = on(s, [](int v) { return v * v; });


### `merge` ###

Take several streams and merge their outputs into a single stream of values. The types of the streams must all be compatible.

    f5::output<int> left = ....
    f5::output<int> right = ....
    f5::output<int> either = f5::merge(left, right);

