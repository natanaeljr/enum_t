/**
 * \file   simple.cc
 * \author Natanael Josué Rabello
 * \brief  Simple example.
 * \date   2019-03-10
 * \copyright Copyright (c) 2019
 */

#include <iostream>
#include <utility>
#include <array>
#include "mxl/enum.h"

/***************************************************************************************/

/**
 * Error code enumeration
 */
enum class Error : char {
    Ok = 0,
    Fail = 1,
    Timeout = -1,
    Internal = 127,
    InvalidParam = -128,
};

/***************************************************************************************/

template<>
constexpr const char* mxl::Enum<Error>::name() const noexcept
{
    switch (enum_) {
        case Error::Ok: return "Ok";
        case Error::Fail: return "Fail";
        case Error::Timeout: return "Timeout";
        case Error::Internal: return "Internal";
        case Error::InvalidParam: return "InvalidParam";
    }
    return nullptr;
}

/***************************************************************************************/

Error function1(int x)
{
    return static_cast<Error>(-(-x % 5));
}

mxl::Enum<Error> function2(int y)
{
    return static_cast<Error>(-(-y % 5));
}

Error foo(Error err)
{
    return static_cast<Error>(99);
}

Error foo(mxl::Enum<Error> err)
{
    return err;
}

/***************************************************************************************/

template<typename E>
constexpr bool is_valid_enum(typename std::underlying_type<E>::type v)
{
    return mxl::Enum<E>(static_cast<E>(v)).name() != nullptr;
}

// template<typename E>
// constexpr int enum_size_impl()
// {
//     return 0;
// }

// template<typename E, typename... N>
// constexpr int enum_size_impl(typename std::underlying_type<E>::type i, N... n)
// {
//     return (is_valid_enum<E>(i) ? 1 : 0) + enum_size_impl<E>(n...);
// }

// template<typename E, typename std::underlying_type<E>::type... N>
// constexpr int enum_size_impl(
//     std::integer_sequence<typename std::underlying_type<E>::type, N...>)
// {
//     return enum_size_impl<E>(N...);
// }

// template<typename E>
// constexpr int enum_size()
// {
//     using enum_type = typename std::underlying_type<E>::type;
//     static_assert(1 == sizeof(enum_type), "Only enumerations of 1-byte size
//     supported"); return enum_size_impl<E>(std::make_integer_sequence<enum_type,
//     127>{});
// }

template<typename E, typename std::underlying_type<E>::type... N>
constexpr auto enum_array_impl_neg(
    std::integer_sequence<typename std::underlying_type<E>::type, N...>)
{
    return ((is_valid_enum<E>((-static_cast<typename std::make_signed<
                                   typename std::underlying_type<E>::type>::type>(N)) -
                              1)
                 ? 1
                 : 0) +
            ...);
}

template<size_t N, size_t... I>
constexpr auto make_array_impl(const std::array<int, sizeof...(I)>& a,
                               std::index_sequence<I...>)
{
    static_assert(N >= sizeof...(I), "");
    return std::array<int, N>{ a[ I ]... };
}

template<size_t N>
constexpr auto operator<<(const std::array<int, N>& a, int x)
{
    std::array<int, N + 1> b = make_array_impl<N + 1>(a, std::make_index_sequence<N>{});
    b[ N ] = x;
    return b;
}

template<typename E, typename std::underlying_type<E>::type... N>
constexpr auto enum_array_impl(
    std::integer_sequence<typename std::underlying_type<E>::type, N...>)
{
    std::array<int, 0> a;
    return ((is_valid_enum<E>(N) ? N : 0) << ...);
}

template<typename E>
constexpr auto enum_array()
{
    using enum_type = typename std::underlying_type<E>::type;
    static_assert(1 == sizeof(enum_type), "Only enumerations of 1-byte size supported");
    constexpr enum_type kMax = (1 << ((sizeof(enum_type) * 8) - 1)) - 1;
    std::make_integer_sequence<enum_type, kMax> seq{};
    // size_t size = enum_array_impl<E>(seq) + (is_valid_enum<E>(kMax) ? 1 : 0);
    auto array = enum_array_impl<E>(seq);
    // if (std::is_signed<enum_type>::value)
    //     size += enum_array_impl_neg<E>(seq) + (is_valid_enum<E>(kMax) ? 1 : 0);
    return array;
}

int main()
{
    // for (int x : { 0, -1, -2, -3, -4 }) {
    //     mxl::Enum<Error> errc = function1(x);
    //     std::cout << "function1 (" << errc.value() << "): " << errc.name() <<
    //     std::endl;
    // }

    // for (int y : { 0, -1, -2, -3, -4 }) {
    //     Error errc = function2(y);
    //     std::cout << "function2 (" << mxl::make_enum(errc).value()
    //               << "): " << mxl::make_enum(errc).name() << std::endl;
    // }

    // std::cout << "sizeof(Error): " << sizeof(Error) << std::endl;
    // std::cout << "sizeof(mxl::Enum<Error>): " << sizeof(mxl::Enum<Error>) << std::endl;

    // std::cout << "foo: " << mxl::make_enum(foo(Error::Ok)).name() << std::endl;

    std::array<int, 2> a{4, 8};
    auto b = a << 1;
    auto c = b << -9;
    for (auto i : c)
        std::cout << i << " ";

    auto ea = enum_array<Error>();

    std::cout << std::endl;
    return 0;
}
