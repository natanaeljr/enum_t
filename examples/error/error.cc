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
    OK = 0,
    FAIL = 2,
    TIMEOUT = -1,
    INTERNAL = 127,
    INVALID_PARAM = -128,
};

/***************************************************************************************/

template<>
constexpr const char* ::njr::enum_t<Error>::name() const
{
    switch (enum_) {
        case Error::OK: return "OK";
        case Error::FAIL: return "FAIL";
        case Error::TIMEOUT: return "TIMEOUT";
        case Error::INTERNAL: return "INTERNAL";
        case Error::INVALID_PARAM: return "INVALID_PARAM";
    }
    return nullptr;
}

/***************************************************************************************/

enum class Register : uint8_t {
    RESET_BIT = 0x1,
    SLEEP_BIT = 0x2,
    ENABLE_BIT = 0x4,
    FIFO_BIT = 0x8,
    OUTPUT_BIT = 0x10,
    COMPACT_BIT = 0x20,
    TRACKING_BIT = 0x40,
    SMART_BIT = 0x80,
};

/***************************************************************************************/

template<>
constexpr const char* ::njr::enum_t<Register>::name() const
{
    switch (enum_) {
        case Register::RESET_BIT: return "RESET_BIT";
        case Register::SLEEP_BIT: return "SLEEP_BIT";
        case Register::ENABLE_BIT: return "ENABLE_BIT";
        case Register::FIFO_BIT: return "FIFO_BIT";
        case Register::OUTPUT_BIT: return "OUTPUT_BIT";
        case Register::COMPACT_BIT: return "COMPACT_BIT";
        case Register::TRACKING_BIT: return "TRACING_BIT";
        case Register::SMART_BIT: return "SMART_BIT";
    }
    return nullptr;
}

static_assert(::njr::is_enum_bitset<Register>::value,
              "Register enum requires bitset trait");

/***************************************************************************************/

int main()
{
    constexpr auto _iee = njr::is_enum_empty<Error>::value;
    constexpr auto _iev = njr::is_enum_valid<Error, 5>::value;
    constexpr auto _iec = njr::is_enum_contiguous<Error>::value;
    constexpr auto _iep = njr::is_enum_zpositive<Error>::value;
    constexpr auto _ien = njr::is_enum_znegative<Error>::value;

    constexpr auto _e1 = njr::enum_t<Error>(Error::OK);
    constexpr auto _e2 = njr::make_enum(Error::OK);
    constexpr njr::enum_t<Error> _e3 = njr::enum_t<Error>(Error::TIMEOUT);
    constexpr njr::enum_t<Error> _e4 = njr::make_enum(Error::TIMEOUT);
    constexpr njr::enum_t<Error> _e5{ Error::FAIL };

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
    static_assert(_error_values.min() == Error::INVALID_PARAM, "");
    static_assert(_error_values[0] == Error::INVALID_PARAM, "");
    static_assert(_error_values[1] == Error::TIMEOUT, "");
    static_assert(_error_values[2] == Error::OK, "");
    static_assert(_error_values[3] == Error::FAIL, "");
    static_assert(_error_values[4] == Error::INTERNAL, "");
    static_assert(_error_values.max() == Error::INTERNAL, "");

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
