/*
 * Copyright (c) Neurala Inc. 2013-2021
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:  The above copyright notice and this
 * permission notice (including the next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef NEURALA_META_DATATYPE_H
#define NEURALA_META_DATATYPE_H

#include <cstdint>

#include "neurala/meta/enum.h"
#include "neurala/neural/DataRange.h"

namespace neurala
{
/**
 * @ingroup NEURALA_META_ENUM
 * @brief Data types.
 */
enum class EDatatype : std::uint8_t
{
	unknown = 0x0,
	/// boolean
	boolean,
	/// unsigned 8-bit integral
	uint8,
	/// unsigned 16-bit integral
	uint16,
	/// 16-bit binary floating-point
	binary16,
	/// 32-bit binary floating-point
	binary32,
	/// 64-bit binary floating-point
	binary64
};


#ifndef SWIG
/// @ingroup NEURALA_META_ENUM
template<>
class MetaEnum<EDatatype> : public MetaEnumRegister<EDatatype>
{
public:
	static constexpr const auto values = enumRegisterValues(
	 NEURALA_META_ENUM_ENTRY(EDatatype, unknown),
	 NEURALA_META_ENUM_ENTRY(EDatatype, boolean),
	 NEURALA_META_ENUM_ENTRY(EDatatype, uint8),
	 NEURALA_META_ENUM_ENTRY(EDatatype, uint16),
	 NEURALA_META_ENUM_ENTRY(EDatatype, binary16),
	 NEURALA_META_ENUM_ENTRY(EDatatype, binary32),
	 NEURALA_META_ENUM_ENTRY(EDatatype, binary64));

	static constexpr const auto fallbackValue = values[0];
};
#endif // SWIG


/**
 * @brief Returns the size of @p datatype.
 */
constexpr std::size_t
sizeofDatatype(EDatatype datatype) noexcept
{
	switch (datatype)
	{
		case EDatatype::boolean:
			return sizeof(bool);
		case EDatatype::uint8:
			return sizeof(std::uint8_t);
		case EDatatype::uint16:
			return sizeof(std::uint16_t);
		case EDatatype::binary16:
			return sizeof(float) / 2u;
		case EDatatype::binary32:
			return sizeof(float);
		case EDatatype::binary64:
			return sizeof(double);
		default:
			return 0u;
	}
}

/**
 * @brief Data type traits.
 */
template<class T>
struct DatatypeTraits;

/**
 * @internal
 * @brief Specialization of @ref DatatypeTraits for @c bool.
 */
template<>
struct DatatypeTraits<bool>
{
	using type = bool;
	static constexpr EDatatype datatype = EDatatype::boolean;
	static constexpr EDataRange datarange = EDataRange::unknown;
	static constexpr std::size_t size = sizeof(type);
};

/**
 * @internal
 * @brief Specialization of @ref DatatypeTraits for @c std::uint8_t.
 */
template<>
struct DatatypeTraits<std::uint8_t>
{
	using type = std::uint8_t;
	static constexpr EDatatype datatype = EDatatype::uint8;
	static constexpr EDataRange datarange = EDataRange::normalizedScaled;
	static constexpr std::size_t size = sizeof(type);
};

/**
 * @internal
 * @brief Specialization of @ref DatatypeTraits for @c std::uint16_t.
 */
template<>
struct DatatypeTraits<std::uint16_t>
{
	using type = std::uint16_t;
	static constexpr EDatatype datatype = EDatatype::uint16;
	static constexpr EDataRange datarange = EDataRange::normalizedScaled;
	static constexpr std::size_t size = sizeof(type);
};

/**
 * @internal
 * @brief Specialization of @ref DatatypeTraits for @c float.
 */
template<>
struct DatatypeTraits<float>
{
	using type = float;
	static constexpr EDatatype datatype = EDatatype::binary32;
	static constexpr EDataRange datarange = EDataRange::normalized;
	static constexpr std::size_t size = sizeof(type);
};

/**
 * @internal
 * @brief Specialization of @ref DatatypeTraits for @c double.
 */
template<>
struct DatatypeTraits<double>
{
	using type = double;
	static constexpr EDatatype datatype = EDatatype::binary64;
	static constexpr EDataRange datarange = EDataRange::normalized;
	static constexpr std::size_t size = sizeof(type);
};

} // namespace neurala

#endif // NEURALA_META_DATATYPE_H
