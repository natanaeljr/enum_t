mxl::enum_t
===========

`mxl::enum_t` is a generic enumeration interface for C++14/17, intended for defining a standard
way of capturing information about an C++ enumeration at compile-time without using macros.

`mxl::enum_t` provides an interface for obtaining further information about an `enum`.
Unlike other reflection enum libraries, `mxl::enum_t` does not rely on macros of any sort to
define your enumeration, in fact, one of the premisses of this library is to not change the way
`enums` are defined. What it wants is to maintain the C++ enum declaration style, and still have
extended information about the enum.

Another premisse of the library is to provide all the enum information as of compile-time so there
are run-time costs when using `mxl::enum_t`, really.


### Features

- [X] Name of the enumerator with `.name()`.
- [X] The size (count) of enumerators.
- [X] A constexpr **iterable** array of the enumerators sorted by value.
- [X] Maximum and minimum values of the enumeration.
- [X] Implicit cast from the `enum` to `mxl::enum`, and vice-versa.
- [X] Enumerator message with `.what()` (defined by user).
- [X] Traits like: `is_enum_valid`, `is_enum_contiguous`, `is_enum_znegative`, `is_enum_zpositive`.

