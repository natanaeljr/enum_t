/**
 * \file   enum.h
 * \author Natanael Josue Rabello
 * \brief  Enum type.
 * \date   2019-03-10
 * \copyright Copyright (c) 2019
 */

#ifndef INCLUDE_NJR_ENUM_T_H_
#define INCLUDE_NJR_ENUM_T_H_

#if (__cplusplus < 201402L)
#error "njr::enum_t only supported since C++14"
#endif

#include <type_traits>
#include <limits>
#include <bitset>
#include <array>

/***************************************************************************************/
namespace njr {

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
    constexpr const char* name() const { return nullptr; }

    /**
     * \brief  Return a custom message detailing the enumerator.
     * \remark This must be implemented for each enum type.
     * \return Enum detail message in C string style.
     */
    constexpr const char* what() const { return nullptr; }

    /**
     * \brief Implicit convertion to the enum type.
     * \return Enum element.
     */
    constexpr operator enum_type() const { return enum_; }

    /**
     * \brief Explicit convertion to the enum value.
     * \return Enum value.
     */
    explicit constexpr operator underlying_type() const { return value(); }

    /**
     * Enumeration's values interface.
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
    static constexpr const bool value = njr::enum_t<E>(E(V)).name() != nullptr;
};

/***************************************************************************************/
/**
 * Library Internals
 */
namespace internal {

template<typename U, typename T, std::size_t S, std::size_t... I>
constexpr auto array_parse_impl(const std::array<T, S>& a, std::index_sequence<I...>)
{
    return std::array<U, S>{ U(a[I])... };
}

template<typename U, typename T, std::size_t S>
constexpr auto array_parse(const std::array<T, S>& a)
{
    return array_parse_impl<U>(a, std::make_index_sequence<S>{});
}

template<typename T, std::size_t S1, std::size_t S2, std::size_t... I1, std::size_t... I2>
constexpr auto array_append_impl(const std::array<T, S1>& a, const std::array<T, S2>& b,
                                 std::index_sequence<I1...>, std::index_sequence<I2...>)
{
    return std::array<T, S1 + S2>{ a[I1]..., b[I2]... };
}

template<typename T, std::size_t S1, std::size_t S2>
constexpr auto array_append(const std::array<T, S1>& a, const std::array<T, S2>& b)
{
    return array_append_impl(a, b, std::make_index_sequence<S1>{},
                             std::make_index_sequence<S2>{});
}

template<typename T, std::size_t S, std::size_t... I>
constexpr auto array_push_front_impl(const std::array<T, S>& a, T x,
                                     std::index_sequence<I...>)
{
    return std::array<T, S + 1>{ x, a[I]... };
}

template<typename T, std::size_t S>
constexpr auto array_push_front(const std::array<T, S>& a, T x)
{
    return array_push_front_impl(a, x, std::make_index_sequence<S>{});
}

template<typename T, std::size_t S, std::size_t... I>
constexpr auto array_push_back_impl(const std::array<T, S>& a, T x,
                                    std::index_sequence<I...>)
{
    return std::array<T, S + 1>{ a[I]..., x };
}

template<typename T, std::size_t S>
constexpr auto array_push_back(const std::array<T, S>& a, T x)
{
    return array_push_back_impl(a, x, std::make_index_sequence<S>{});
}

/***************************************************************************************/

#if (__cplusplus >= 201703L)

template<typename E, typename std::underlying_type<E>::type V, std::size_t S>
constexpr auto make_enum_array17_impl(
    const std::array<typename std::underlying_type<E>::type, S>& a)
{
    if constexpr (V != 0 && V != -1) {
        if constexpr (!is_enum_valid<E, V>::value)
            return make_enum_array17_impl<E, V + (V < 0 ? 1 : -1)>(a);
        if constexpr (is_enum_valid<E, V>::value)
            return make_enum_array17_impl<E, V + (V < 0 ? 1 : -1)>(
                V < 0 ? array_push_back(a, V) : array_push_front(a, V));
    }
    else {
        if constexpr (!is_enum_valid<E, V>::value)
            return a;
        else if constexpr (is_enum_valid<E, V>::value)
            return V < 0 ? array_push_back(a, V) : array_push_front(a, V);
    }
}

template<typename E>
constexpr auto make_enum_array17()
{
    using enum_underlying_t = typename std::underlying_type<E>::type;
    if constexpr (std::is_signed<enum_underlying_t>::value) {
        using enum_numeric = std::numeric_limits<char>;
        return array_append(make_enum_array17_impl<E, enum_numeric::min()>(
                                std::array<enum_underlying_t, 0>{}),
                            make_enum_array17_impl<E, enum_numeric::max()>(
                                std::array<enum_underlying_t, 0>{}));
    }
    else {
        using enum_numeric = std::numeric_limits<unsigned char>;
        return make_enum_array17_impl<E, enum_numeric::max()>(
            std::array<enum_underlying_t, 0>{});
    }
}

template<typename E>
inline constexpr auto make_enum_array()
{
    return make_enum_array17<E>();
}

/***************************************************************************************/
#elif (__cplusplus >= 201402L) /* #if (__cplusplus >= 201703L) */

template<typename E, typename std::underlying_type<E>::type V,
         typename std::enable_if<!is_enum_valid<E, V>::value && (V == 0 || V == -1),
                                 std::size_t>::type S>
constexpr auto make_enum_array14_impl(
    const std::array<typename std::underlying_type<E>::type, S>& a)
{
    return a;
}

template<
    typename E, typename std::underlying_type<E>::type V,
    typename std::enable_if<is_enum_valid<E, V>::value && (V == -1), std::size_t>::type S>
constexpr auto make_enum_array14_impl(
    const std::array<typename std::underlying_type<E>::type, S>& a)
{
    return array_push_back(a, V);
}

template<
    typename E, typename std::underlying_type<E>::type V,
    typename std::enable_if<is_enum_valid<E, V>::value && (V == 0), std::size_t>::type S>
constexpr auto make_enum_array14_impl(
    const std::array<typename std::underlying_type<E>::type, S>& a)
{
    return array_push_front(a, V);
}

template<
    typename E, typename std::underlying_type<E>::type V,
    typename std::enable_if<!is_enum_valid<E, V>::value && (V < -1), std::size_t>::type S>
constexpr auto make_enum_array14_impl(
    const std::array<typename std::underlying_type<E>::type, S>& a)
{
    return make_enum_array14_impl<E, V + 1>(a);
}

/* Forward-declaration */
template<
    typename E, typename std::underlying_type<E>::type V,
    typename std::enable_if<is_enum_valid<E, V>::value && (V > 0), std::size_t>::type S>
constexpr auto make_enum_array14_impl(
    const std::array<typename std::underlying_type<E>::type, S>& a);

template<
    typename E, typename std::underlying_type<E>::type V,
    typename std::enable_if<!is_enum_valid<E, V>::value && (V > 0), std::size_t>::type S>
constexpr auto make_enum_array14_impl(
    const std::array<typename std::underlying_type<E>::type, S>& a)
{
    return make_enum_array14_impl<E, V - 1>(a);
}

template<
    typename E, typename std::underlying_type<E>::type V,
    typename std::enable_if<is_enum_valid<E, V>::value && (V < -1), std::size_t>::type S>
constexpr auto make_enum_array14_impl(
    const std::array<typename std::underlying_type<E>::type, S>& a)
{
    return make_enum_array14_impl<E, V + 1>(array_push_back(a, V));
}

template<
    typename E, typename std::underlying_type<E>::type V,
    typename std::enable_if<is_enum_valid<E, V>::value && (V > 0), std::size_t>::type S>
constexpr auto make_enum_array14_impl(
    const std::array<typename std::underlying_type<E>::type, S>& a)
{
    return make_enum_array14_impl<E, V - 1>(array_push_front(a, V));
}

template<typename E, typename std::enable_if<
                         std::is_signed<typename std::underlying_type<E>::type>::value,
                         std::size_t>::type D = 0>
constexpr auto make_enum_array14()
{
    using enum_numeric = std::numeric_limits<char>;
    using enum_underlying_t = typename std::underlying_type<E>::type;
    return array_append(make_enum_array14_impl<E, enum_numeric::min()>(
                            std::array<enum_underlying_t, 0>{}),
                        make_enum_array14_impl<E, enum_numeric::max()>(
                            std::array<enum_underlying_t, 0>{}));
}

template<typename E, typename std::enable_if<
                         std::is_unsigned<typename std::underlying_type<E>::type>::value,
                         void>::type D = 0>
constexpr auto make_enum_array14()
{
    using enum_numeric = std::numeric_limits<unsigned char>;
    return make_enum_array14_impl<E, enum_numeric::max()>(
        std::array<typename std::underlying_type<E>::type, 0>{});
}

template<typename E>
inline constexpr auto make_enum_array()
{
    return make_enum_array14<E>();
}

/***************************************************************************************/
#endif                         // #elif (__cplusplus >= 201402L)

} /* namespace internal */

/***************************************************************************************/

template<typename E>
class enum_t<E>::values final {
   private:
    static constexpr const auto array_ =
        internal::array_parse<enum_t<E>>(internal::make_enum_array<E>());

   public:
    using array_type = decltype(array_);
    using enum_type = E;
    using njr_enum_type = enum_t<E>;
    using size_type = typename array_type::size_type;

    static constexpr array_type array() { return array_; }
    static constexpr size_type count() { return array_.size(); }
    static constexpr njr_enum_type min()
    {
        return count() ? array_.front() : njr_enum_type{ 0 };
    }
    static constexpr njr_enum_type max()
    {
        return count() ? array_.back() : njr_enum_type{ 0 };
    }

    constexpr operator array_type() const { return array_; }
    constexpr array_type operator()() const { return array_; }
    constexpr njr_enum_type operator[](size_type i) const { return array_[i]; }
};

/***************************************************************************************/
/**
 * Library Internals
 */
namespace internal {

template<typename E, std::size_t S>
constexpr auto is_enum_contiguous(const std::array<njr::enum_t<E>, S>& a)
{
    if (S == 0)
        return false;
    else if (S == 1)
        return true;
    for (std::size_t i = 1; i != S; ++i)
        if (a[i].value() - 1 != a[i - 1].value())
            return false;
    return true;
}

template<typename E, std::size_t S>
constexpr auto is_enum_zpositive(const std::array<njr::enum_t<E>, S>& a)
{
    if (S == 0)
        return false;
    for (std::size_t i = 0; i != S; ++i)
        if (a[i].value() < 0)
            return false;
    return true;
}

template<typename E, std::size_t S>
constexpr auto is_enum_znegative(const std::array<njr::enum_t<E>, S>& a)
{
    if (S == 0)
        return false;
    for (std::size_t i = 0; i != S; ++i)
        if (a[i].value() > 0)
            return false;
    return true;
}

template<typename E, std::size_t S>
constexpr auto is_enum_bitset(const std::array<njr::enum_t<E>, S>& a, bool nonzero)
{
    if (S == 0)
        return false;
    for (std::size_t i = 0; i != S; ++i) {
        std::size_t c = 0;
        for (std::size_t b = 1; b != 1 << 9 && c < 2; b = b << 1)
            c += (a[i].value() & b) ? 1 : 0;
        if (c > 1)
            return false;
        if (nonzero && c == 0)
            return false;
    }
    return true;
}

} /* namespace internal */

/***************************************************************************************/
/*
 * Enum Traits
 */
template<typename E>
struct is_enum_empty {
    static constexpr const bool value = (njr::enum_t<E>::values::count() == 0);
};

template<typename E>
struct is_enum_contiguous {
    static constexpr const bool value =
        internal::is_enum_contiguous(njr::enum_t<E>::values::array());
};

template<typename E>
struct is_enum_zpositive {
    static constexpr const bool value =
        internal::is_enum_zpositive(njr::enum_t<E>::values::array());
};

template<typename E>
struct is_enum_znegative {
    static constexpr const bool value =
        internal::is_enum_znegative(njr::enum_t<E>::values::array());
};

template<typename E>
struct is_enum_bitset {
    static constexpr const bool value =
        internal::is_enum_bitset(njr::enum_t<E>::values::array(), true);
};

template<typename E>
struct is_enum_zbitset {
    static constexpr const bool value =
        internal::is_enum_bitset(njr::enum_t<E>::values::array(), false);
};

} /* namespace njr */

#endif /* INCLUDE_NJR_ENUM_T_H_ */
