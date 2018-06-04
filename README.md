# pipeline

Pipeline is a library for C++ that supports the creation of "data networks" (pipelines), to control the flow of values between various components of the program logically and functionally.

## Concepts

The library is built around the concept of data **sources** and data **sinks**. Sources are structures that can push data to sinks; likewise, sinks are structures that can accept data from sources. This flow of data is controlled by binding sinks to sources; a sink can only be bound to one source, but many sinks can be bound to the same source, creating a `n->1` relationship.

## API

In general, a value representing a sink can be bound to a value representing a source using a pipe symbol:

    source | sink;

Sources and sinks can be formal or informal. A formal source or sink is represented by an object of a subclass of `pipeline::source<T>` or `pipeline::sink<T>`, respectively. An informal source or sink could be a constant or a function (i.e. not an instance of `pipeline::source<T>` or `pipeline::sink<T>`).
