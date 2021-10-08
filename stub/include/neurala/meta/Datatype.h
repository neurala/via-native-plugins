/*
 * This file is part of Neurala SDK.
 * Copyright Neurala Inc. 2013-2021. All rights reserved.
 *
 * Except as expressly permitted in the accompanying License Agreement, if at all, (a) you shall
 * not license, sell, rent, lease, transfer, assign, distribute, display, host, outsource, disclose
 * or otherwise commercially exploit or make this source code available to any third party; (b) you
 * shall not modify, make derivative works of, disassemble, reverse compile or reverse engineer any
 * part of the SDK; (c) You shall not access the SDK in order to build a similar or competitive
 * product or service; (d) no part of the this source may be copied, reproduced, distributed,
 * republished, downloaded, displayed, posted or transmitted in any form or by any means, including
 * but not limited to electronic, mechanical, photocopying, recording or other means; and (e) any
 * future release, update, or other addition to functionality of the SDK shall be subject to the
 * terms of the accompanying License Agreement. You must reproduce, on all copies made by you or
 * for you, and must not remove, alter, or obscure in any way all proprietary rights notices
 * (including copyright notices) of Neurala Inc or its suppliers on or within the copies of the
 * SDK. Any sample code provided with the SDK and designated as such are for illustrative purposes
 * only and are not to be included in your applications.
 *
 * In cases when the accompanying License Agreement permits redistribution of this file, the above
 * notice shall be reproduced its entirety in every copy of a distributed version of this file.
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


// NOTE:20210927:jgerity:SWIG does not support `auto` without a trailing return type declaration (http://www.swig.org/Doc4.0/SWIGDocumentation.html#CPlusPlus11_alternate_function_syntax)
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
#endif  // SWIG


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
