/**
 * \file   enum.h
 * \author Natanael Josué Rabello
 * \brief  MXL Enum.
 * \date   2019-03-10
 * \copyright Copyright (c) 2019
 */

#ifndef INCLUDE_MXL_ENUM_H_
#define INCLUDE_MXL_ENUM_H_

#include <type_traits>

/***************************************************************************************/

/**
 * Minimum Extension Library
 */
namespace mxl {

/**
 * Enumeration convenience class.
 * Provides Enum names and implicit convertion from enum value to its underlying type.
 */
template<typename E>
class Enum {
   public:
    static_assert(std::is_enum<E>::value, "Template parameter must be of enum type");
    //! Alias to enum type
    using type = E;
    //! Underlying type of this enum
    using underlying_type = typename std::underlying_type<E>::type;

    /**
     * \brief Enum constructor by element.
     * \param enumerator Enum element of this enumerator.
     */
    constexpr Enum(E enumerator) noexcept : enum_{ enumerator } {}

    /**
     * \brief Enum constructor by value.
     * \param value Enum value of this enumerator.
     */
    constexpr explicit Enum(underlying_type value) : enum_{ static_cast<E>(value) } {}

    /**
     * \brief  Return the name of the enumerator.
     * \detail This must be implemented for each enum type.
     * \return Enum name in C string style.
     */
    constexpr const char* name() const noexcept;

    /**
     * \brief Return the enum value of this object.
     * \return Enum value.
     */
    constexpr underlying_type value() const noexcept
    {
        return static_cast<underlying_type>(enum_);
    }

    /**
     * \brief Implicit convertion to the enum type.
     * \return Enum element.
     */
    constexpr operator E() const noexcept { return enum_; }

   private:
    /*! Representing enum element */
    E enum_;
};

/**
 * \brief Parse an enum to the Enum convenience class.
 * \param enumerator Enum value.
 * \return Enumeration object.
 */
template<typename E>
static constexpr Enum<E> make_enum(E enumerator)
{
    return Enum<E>(enumerator);
}

} /* namespace mxl */

#endif /* INCLUDE_MXL_ENUM_H_ */
