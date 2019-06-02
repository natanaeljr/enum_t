njr::enum_t
======

`enum_t` is a generic enumeration interface for C++14/17, intended for defining a standard
way of capturing information about an enumeration at compile-time **without using macros** like other
enum libraries do.

`enum_t` provides an interface for obtaining information about an `enum` type.
Unlike other reflection enum libraries, `enum_t` does not rely on macros of any sort to
define your enumeration, in fact, one of the premisses of this library is to not change the way
`enum`s are defined. What it wants is to maintain the C++ enum declaration style, and still have
extended information about the enum.

Another premisse of the library is all the enum information should be available as of compile-time
so there is no run-time cost when using `enum_t`, really!


### Features

- [X] Enumerators' **name** with `enum_t<E>::name()`.
- [X] The **size** (count) of enumerators.
- [X] Maximum and minimum values of the enumeration.
- [X] Constexpr **iterable array** of the enumerator sorted by value, with `enum_t<E>::values`
- [X] Implicit interchangeable convertion from `enum` to `enum_t<E>`, and vice-versa.
- [X] **Enum traits**: `is_enum_valid`, `is_enum_contiguous`, `is_enum_znegative`, `is_enum_zpositive`,
`is_enum_bitset`.
- [X] Enumerator's meaning with `enum_t<E>::what()`.

