/**
 * \file   simple.cc
 * \author Natanael Josué Rabello
 * \brief  Simple example.
 * \date   2019-03-10
 * \copyright Copyright (c) 2019
 */

#include <iostream>
#include <utility>
#include <limits>
#include <array>
#include "mxl/enum.h"

/***************************************************************************************/

/**
 * Error code enumeration
 */
enum class Error : char {
    Ok = 0,
    Fail = 2,
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

// Error function1(int x)
// {
//     return static_cast<Error>(-(-x % 5));
// }

// mxl::Enum<Error> function2(int y)
// {
//     return static_cast<Error>(-(-y % 5));
// }

// Error foo(Error err)
// {
//     return static_cast<Error>(99);
// }

// Error foo(mxl::Enum<Error> err)
// {
//     return err;
// }

/***************************************************************************************/

template<typename E, typename std::underlying_type<E>::type V>
struct is_enum_valid {
    inline static constexpr const bool value = mxl::Enum<E>(E(V)).name() != nullptr;
};

template<typename T, size_t S1, size_t S2, size_t... I1, size_t... I2>
constexpr auto array_append_impl(const std::array<T, S1>& a, const std::array<T, S2>& b,
                                 std::index_sequence<I1...>, std::index_sequence<I2...>)
{
    return std::array<T, S1 + S2>{ a[I1]..., b[I2]... };
}

template<typename T, size_t S1, size_t S2>
constexpr auto array_append(const std::array<T, S1>& a, const std::array<T, S2>& b)
{
    return array_append_impl(a, b, std::make_index_sequence<S1>{},
                             std::make_index_sequence<S2>{});
}

template<typename T, size_t S1, size_t S2>
constexpr auto operator<<(const std::array<T, S1>& a, const std::array<T, S2>& b)
{
    return array_append(a, b);
}

template<typename T, size_t S, size_t... I>
constexpr auto array_push_front_impl(const std::array<T, S>& a, T x,
                                     std::index_sequence<I...>)
{
    return std::array<T, S + 1>{ x, a[I]... };
}

template<typename T, size_t S>
constexpr auto array_push_front(const std::array<T, S>& a, T x)
{
    return array_push_front_impl(a, x, std::make_index_sequence<S>{});
}

template<typename T, size_t S>
constexpr auto operator<<(const std::array<T, S>& a, T x)
{
    return array_push_front(a, x);
}

/*
// Prototype 1
template<
    typename E, int V, int D, size_t S,
    typename std::enable_if<is_enum_valid<E, V>::value && V != 0, size_t>::type Z = 0>
constexpr auto make_enum_array(const std::array<int, S>& a);

// Definitions
template<typename E, int V, int D, size_t S,
         typename std::enable_if<!is_enum_valid<E, V>::value && V == -1 && D == 1,
                                 size_t>::type Z = 0>
constexpr auto make_enum_array(const std::array<int, S>& a)
{
    return a;
}

template<typename E, int V, int D, size_t S,
         typename std::enable_if<is_enum_valid<E, V>::value && V == -1 && D == 1,
                                 size_t>::type Z = 0>
constexpr auto make_enum_array(const std::array<int, S>& a)
{
    return array_push_front(a, V);
}

template<
    typename E, int V, int D, size_t S,
    typename std::enable_if<!is_enum_valid<E, V>::value && V == 0, size_t>::type Z = 0>
constexpr auto make_enum_array(const std::array<int, S>& a)
{
    return a;
}

template<
    typename E, int V, int D, size_t S,
    typename std::enable_if<is_enum_valid<E, V>::value && V == 0, size_t>::type Z = 0>
constexpr auto make_enum_array(const std::array<int, S>& a)
{
    return array_push_front(a, V);
}

template<typename E, int V, int D, size_t S,
         typename std::enable_if<!is_enum_valid<E, V>::value && V != 0 && V != -1,
                                 size_t>::type Z = 0>
constexpr auto make_enum_array(const std::array<int, S>& a)
{
    return make_enum_array<E, V + D, D>(a);
}

template<typename E, int V, int D, size_t S,
         typename std::enable_if<is_enum_valid<E, V>::value && V != 0 && V != -1,
                                 size_t>::type Z>
constexpr auto make_enum_array(const std::array<int, S>& a)
{
    return make_enum_array<E, V + D, D>(array_push_front(a, V));
}

template<typename E>
constexpr auto make_enum_array()
{
    auto pos_enum_array = make_enum_array<E, 127, -1>(std::array<int, 0>{});
    auto neg_enum_array = make_enum_array<E, -128, 1>(std::array<int, 0>{});
    return array_append(pos_enum_array, neg_enum_array);
}
*/

#if (__cplusplus >= 201400L)

#endif /* #if (__cplusplus >= 201400L) */

/***************************************************************************************/

#if (__cplusplus >= 201700L)

template<typename E, typename std::underlying_type<E>::type V, size_t S>
constexpr auto make_enum_array17_impl(const std::array<mxl::Enum<E>, S>& a)
{
    if constexpr (V != 0 && V != -1) {
        if constexpr (!is_enum_valid<E, V>::value)
            return make_enum_array17_impl<E, V + (V < 0 ? 1 : -1)>(a);
        if constexpr (is_enum_valid<E, V>::value)
            return make_enum_array17_impl<E, V + (V < 0 ? 1 : -1)>(a
                                                                   << mxl::Enum<E>(E(V)));
    }
    else {
        if constexpr (!is_enum_valid<E, V>::value)
            return a;
        else if constexpr (is_enum_valid<E, V>::value)
            return a << mxl::Enum<E>(E(V));
    }
}

template<typename E>
constexpr auto make_enum_array17()
{
    if constexpr (std::is_signed<typename std::underlying_type<E>::type>::value) {
        using enum_numeric = std::numeric_limits<char>;
        return array_append(
            make_enum_array17_impl<E, enum_numeric::min()>(std::array<mxl::Enum<E>, 0>{}),
            make_enum_array17_impl<E, enum_numeric::max()>(
                std::array<mxl::Enum<E>, 0>{}));
    }
    else {
        using enum_numeric = std::numeric_limits<unsigned char>;
        return make_enum_array17_impl<E, enum_numeric::max()>(
            std::array<mxl::Enum<E>, 0>{});
    }
}

#endif /* #if (__cplusplus >= 201700L) */

// template<typename E, int... N>
// constexpr auto enum_compose_impl(int I, )
// {
//     return enum_compose_impl
// }

// template<typename E, size_t... N>
// constexpr auto enum_compose_impl(std::index_sequence<N...>)
// {
// }

// template<typename E>
// constexpr auto enum_compose()
// {
//     return enum_compose_impl<E>(std::make_index_sequence<127>{});
// }

// template<typename E>
// constexpr int enum_size_impl()
// {
//     return 0;
// }

// template<typename E, typename... N>
// constexpr int enum_size_impl(typename std::underlying_type<E>::type i, N... n)
// {
//     return (is_enum_valid<E, i>::value ? 1 : 0) + enum_size_impl<E>(n...);
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
//     static_assert(1 == sizeof(enum_type), "Only enumerations of 1-byte size supported
//     "); return enum_size_impl<E>(std::make_integer_sequence<enum_type, 127>{});
// }

// template<typename E, typename std::underlying_type<E>::type... N>
// constexpr auto enum_array_impl_neg(
//     std::integer_sequence<typename std::underlying_type<E>::type, N...>)
// {
//     return ((is_enum_valid<E, (-static_cast<typename std::make_signed<
//                                    typename std::underlying_type<E>::type>::type>(N)) -
//                                   1>::value
//                  ? 1
//                  : 0) +
//             ...);
// }

// template<size_t N, size_t... I>
// constexpr auto make_array_impl(const std::array<int, sizeof...(I)>& a,
//                                std::index_sequence<I...>)
// {
//     static_assert(N >= sizeof...(I), "");
//     return std::array<int, N>{ a[ I ]... };
// }

// template<size_t N>
// constexpr auto operator<<(const std::array<int, N>& a, int x)
// {
//     std::array<int, N + 1> b = make_array_impl<N + 1>(a,
//     std::make_index_sequence<N>{}); b[ N ] = x; return b;
// }

// template<typename E, typename std::underlying_type<E>::type... N>
// constexpr auto enum_array_impl(
//     std::integer_sequence<typename std::underlying_type<E>::type, N...>)
// {
//     std::array<int, 0> a;
//     return ((is_enum_valid<E, N>::value ? N : 0) << ...);
// }

// template<typename E>
// constexpr auto enum_array()
// {
//     using enum_type = typename std::underlying_type<E>::type;
//     static_assert(1 == sizeof(enum_type), "Only enumerations of 1-byte size
//     supported"); constexpr enum_type kMax = (1 << ((sizeof(enum_type) * 8) - 1)) - 1;
//     std::make_integer_sequence<enum_type, kMax> seq{};
//     // size_t size = enum_array_impl<E>(seq) + (is_enum_valid<E>(kMax) ? 1 : 0);
//     auto array = enum_array_impl<E>(seq);
//     // if (std::is_signed<enum_type>::value)
//     //     size += enum_array_impl_neg<E>(seq) + (is_enum_valid<E>(kMax) ? 1 : 0);
//     return array;
// }

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
#if (__cplusplus >= 201700L)
    auto x = make_enum_array17<Error>();
#elif (__cplusplus >= 201700L)
    auto x = make_enum_array14<Error>();
#endif

    for (auto e : x)
        std::cout << e.name() << ": " << (int) e.value() << '\n';

    std::cout << std::endl;
    return 0;
}
