# Gorilla Core
Gorilla Core is cross platform library offering light & fast standard c++ classes.

# Sources organization
## Container
Severals data structure designed to hold data in a contigous memory block limiting heap allocations. `Vector` and `HashMap` are classic but fast. `Dictionary` is a fast tree structure allowing toi output different format such as binary, json, csv.

## File System
Regrouping I/O operations using file access through `FileSystem` class. A file reprensentation is provided by `File`. You have also the possibility to use `Path` to format, combine or extract information needed.

## Log
You can easily log in your application using severals defines that will provide default formating

    LOG_MESSAGE
    LOG_WARNING
    LOG_ERROR
    LOG_ASSERT

## Memory

A memory manager is provided to check and follow all allocations of your applications. It can also be used as a statistics on different allocations category.

## Process
Create a subprocess and get result from. An agurment parser is also present to easily parse command line arguments

## Stream
Input/Output streams in file or memory. Specific streams format are also present for the dictionary such as binary, json and csv.

## String
Support ASCII, UTF8 and UTF16 using `String` and `String16`. You can also make convertion between those using `StringHelper`. This class also provide numbers conversion from/to string.

## Time
Contains classes related to times aiming profiling and update system time based.

# Supported Platorms
    - Windows
    - macOS
    - Linux
