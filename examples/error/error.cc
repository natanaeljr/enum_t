/**
 * \file   error.cc
 * \author Natanael Josue Rabello
 * \brief  Example with Error enum.
 * \date   2019-03-10
 * \copyright Copyright (c) 2019
 */

#include <iostream>
#include "njr/enum_t.h"

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
constexpr const char* ::njr::enum_t<Error>::name() const
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

int main()
{
    constexpr auto _iee = njr::is_enum_empty<Error>::value;
    constexpr auto _iev = njr::is_enum_valid<Error, 5>::value;
    constexpr auto _iec = njr::is_enum_contiguous<Error>::value;
    constexpr auto _iep = njr::is_enum_zpositive<Error>::value;
    constexpr auto _ien = njr::is_enum_znegative<Error>::value;

    constexpr auto _e1 = njr::enum_t<Error>(Error::Ok);
    constexpr auto _e2 = njr::make_enum(Error::Ok);
    constexpr njr::enum_t<Error> _e3 = njr::enum_t<Error>(Error::Timeout);
    constexpr njr::enum_t<Error> _e4 = njr::make_enum(Error::Timeout);
    constexpr njr::enum_t<Error> _e5{ Error::Fail };

    constexpr auto _emin = njr::enum_t<Error>::values::min();
    constexpr auto _emax = njr::enum_t<Error>::values::max();
    constexpr auto _evc = njr::enum_t<Error>::values::count();
    constexpr auto _eva = njr::enum_t<Error>::values::array();
    constexpr njr::enum_t<Error>::values _error_values;

    static_assert(_iee == false, "");
    static_assert(_iev == false, "");
    static_assert(_iec == false, "");
    static_assert(_iep == false, "");
    static_assert(_ien == false, "");
    static_assert(_error_values.min() == Error::InvalidParam, "");
    static_assert(_error_values[0] == Error::InvalidParam, "");
    static_assert(_error_values[1] == Error::Timeout, "");
    static_assert(_error_values[2] == Error::Ok, "");
    static_assert(_error_values[3] == Error::Fail, "");
    static_assert(_error_values[4] == Error::Internal, "");
    static_assert(_error_values.max() == Error::Internal, "");

    static_assert(_evc == 5, "");

    std::cout << "=== Error ===" << '\n';

    std::cout << "Count: " << _evc << '\n';
    std::cout << "Min: " << (int) _emin.value() << '\n';
    std::cout << "Max: " << (int) _emax.value() << '\n';

    std::cout << "Values:" << '\n';
    for (auto e : _eva)
        std::cout << "  " << e.name() << ": " << (int) e.value() << '\n';
    std::cout << '\n';

    std::cout << "Contiguous: " << std::boolalpha << _iec << '\n';

    std::cout << std::endl;
    return 0;
}
