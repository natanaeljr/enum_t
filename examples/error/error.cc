/**
 * \file   simple.cc
 * \author Natanael Josué Rabello
 * \brief  Simple example.
 * \date   2019-03-10
 * \copyright Copyright (c) 2019
 */

#include <iostream>
#include <utility>
#include "mxl/enum.h"

/***************************************************************************************/

/**
 * Error code enumeration
 */
enum class Error {
    Ok = 0,
    Fail = -1,
    Timeout = -2,
    Internal = -3,
    InvalidParam = -4,
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
constexpr bool is_valid_enum(int x)
{
    return mxl::make_enum(static_cast<E>(x)).name() != nullptr;
}


int main()
{
    for (int x : { 0, -1, -2, -3, -4 }) {
        mxl::Enum<Error> errc = function1(x);
        std::cout << "function1 (" << errc.value() << "): " << errc.name() << std::endl;
    }

    for (int y : { 0, -1, -2, -3, -4 }) {
        Error errc = function2(y);
        std::cout << "function2 (" << mxl::make_enum(errc).value()
                  << "): " << mxl::make_enum(errc).name() << std::endl;
    }

    std::cout << "sizeof(Error): " << sizeof(Error) << std::endl;
    std::cout << "sizeof(mxl::Enum<Error>): " << sizeof(mxl::Enum<Error>) << std::endl;

    // std::cout << "foo: " << mxl::make_enum(foo(Error::Ok)).name() << std::endl;

    std::make_index_sequence<sizeof(int)> x;
    bool is = is_valid_enum<Error>(-5);
    std::cout << "is_valid_enum: " << is << std::endl;

    return 0;
}
