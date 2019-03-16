/**
 * \file   enum.h
 * \author Natanael Josué Rabello
 * \brief  MXL Enum.
 * \date   2019-03-10
 * \copyright Copyright (c) 2019
 */

#ifndef INCLUDE_MXL_ENUM_H_
#define INCLUDE_MXL_ENUM_H_

#if (__cplusplus < 201400L)
#error "MXL Enum only supported since C++14"
#endif

#include <type_traits>
#include <limits>
#include <array>

/***************************************************************************************/
/**
 * Minimum Extension Library
 */
namespace mxl {

/**
 * Enumerator class.
 * Provides Enum names and implicit convertion from enum value to its underlying type.
 */
template<typename E>
class enum_t final {
    static_assert(std::is_enum<E>::value, "Template parameter must be of enum type");

   public:
    //! Alias to enum type
    using enum_type = E;
    //! Underlying type of this enum
    using underlying_type = typename std::underlying_type<E>::type;

    /**
     * \brief Enum constructor by element.
     * \param enumerator Enum element of this enumerator.
     */
    constexpr enum_t(E enumerator) : enum_{ enumerator } {}

    /**
     * \brief Enum constructor by value.
     * \note  Generic implementation does validate the input value.
     * \param value Enum value of this enumerator.
     */
    constexpr explicit enum_t(underlying_type value) : enum_{ E(value) } {}

    /**
     * \brief Return the enum value of this object.
     * \return Enum value.
     */
    constexpr underlying_type value() const
    {
        return static_cast<underlying_type>(enum_);
    }

    /**
     * \brief  Return the name of the enumerator.
     * \remark This must be implemented for each enum type.
     * \return Enum name in C string style.
     */
    constexpr const char* name() const;

    /**
     * \brief  Return a custom message detailing the enumerator.
     * \remark This must be implemented for each enum type.
     * \return Enum detail message in C string style.
     */
    constexpr const char* message() const { return "(undefined enum message)"; }

    /**
     * \brief Implicit convertion to the enum type.
     * \return Enum element.
     */
    constexpr operator enum_type() const { return enum_; }

    /**
     * Enumeration's value information interface.
     */
    class values;

   private:
    /*! Representing enum element */
    enum_type enum_;
};

/**
 * \brief Parse an enum to the Enum convenience class.
 * \param enumerator Enum value.
 * \return Enumeration object.
 */
template<typename E>
constexpr enum_t<E> make_enum(E enumerator)
{
    return enum_t<E>(enumerator);
}

/**
 * \brief Check if a constexpr value is a valid declared enumerator value.
 * \tparam E  Enumerator type.
 * \tparam V  Value queried.
 */
template<typename E, typename std::underlying_type<E>::type V>
struct is_enum_valid {
    static constexpr const bool value = mxl::enum_t<E>(E(V)).name() != nullptr;
};

/***************************************************************************************/
/**
 * Library Internals
 */
namespace internal {

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

/***************************************************************************************/

#if (__cplusplus >= 201700L)

template<typename E, typename std::underlying_type<E>::type V, size_t S>
constexpr auto make_enum_array17_impl(const std::array<mxl::enum_t<E>, S>& a)
{
    if constexpr (V != 0 && V != -1) {
        if constexpr (!is_enum_valid<E, V>::value)
            return make_enum_array17_impl<E, V + (V < 0 ? 1 : -1)>(a);
        if constexpr (is_enum_valid<E, V>::value)
            return make_enum_array17_impl<E, V + (V < 0 ? 1 : -1)>(
                array_push_front(a, mxl::enum_t<E>(E(V))));
    }
    else {
        if constexpr (!is_enum_valid<E, V>::value)
            return a;
        else if constexpr (is_enum_valid<E, V>::value)
            return array_push_front(a, mxl::enum_t<E>(E(V)));
    }
}

template<typename E>
constexpr auto make_enum_array17()
{
    if constexpr (std::is_signed<typename std::underlying_type<E>::type>::value) {
        using enum_numeric = std::numeric_limits<char>;
        return array_append(make_enum_array17_impl<E, enum_numeric::min()>(
                                std::array<mxl::enum_t<E>, 0>{}),
                            make_enum_array17_impl<E, enum_numeric::max()>(
                                std::array<mxl::enum_t<E>, 0>{}));
    }
    else {
        using enum_numeric = std::numeric_limits<unsigned char>;
        return make_enum_array17_impl<E, enum_numeric::max()>(
            std::array<mxl::enum_t<E>, 0>{});
    }
}

/***************************************************************************************/
#elif (__cplusplus >= 201400L) /* #if (__cplusplus >= 201700L) */

// TODO: C++14 support

/***************************************************************************************/
#endif                         /* #elif (__cplusplus >= 201400L) */

} /* namespace internal */

/***************************************************************************************/

template<typename E>
class enum_t<E>::values final {
   private:
    static constexpr const auto array_ = internal::make_enum_array17<E>();

   public:
    using array_type = decltype(array_);
    using enum_type = E;
    using mxl_enum_type = enum_t<E>;
    using size_type = typename array_type::size_type;

    static constexpr array_type array() { return array_; }
    static constexpr size_type count() { return array_.size(); }
    static constexpr mxl_enum_type min() { return array_.front(); }
    static constexpr mxl_enum_type max() { return array_.back(); }

    constexpr operator array_type() { return array_; }
    constexpr array_type operator()() { return array_; }
};

} /* namespace mxl */

#endif /* INCLUDE_MXL_ENUM_H_ */
