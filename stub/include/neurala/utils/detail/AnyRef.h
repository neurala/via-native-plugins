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

#ifndef NEURALA_UTILS_DETAIL_ANY_REF_H
#define NEURALA_UTILS_DETAIL_ANY_REF_H

#include <cstring>
#include <type_traits>
#include <typeindex>

#include "neurala/utils/string.h"
#include "neurala/meta/detail/demangle.h"
#include "neurala/meta/detail/typeName.h"

namespace neurala
{
/**
 * @brief Type-erased reference wrapper.
 *
 * It type erases the argument that it stores, but performs type checking at retrieval to avoid
 * incorrect casts at run-time.
 */
class AnyRef
{
	enum EQualifier
	{
		notQualified = 0x0,
		constQualified = 0x1,
		volatileQualified = 0x2
	};

	void* m_data{};
	std::type_index m_type;
	EQualifier m_qualifiers{notQualified};

public:
	template<class T>
	AnyRef(T& t) noexcept
	 : m_data(const_cast<void*>(static_cast<const void*>(&t))),
	   m_type(typeid(std::remove_reference_t<T>)),
	   m_qualifiers(static_cast<EQualifier>(
	    (std::is_const<std::remove_reference_t<T>>::value ? constQualified : notQualified)
	    | (std::is_volatile<std::remove_reference_t<T>>::value ? volatileQualified : notQualified)))
	{}

	/**
	 * @brief Returns if the argument is of type @p T.
	 */
	template<class T>
	bool isOfType() const noexcept
	{
		// if reference is cv-qualified, qualifiers need to match
		if (((m_qualifiers & constQualified) != 0x0 && !std::is_const<T>::value)
		    || ((m_qualifiers & volatileQualified) != 0x0 && !std::is_volatile<T>::value))
		{
			return false;
		}

		// check the name
		return std::strcmp(typeid(T).name(), m_type.name()) == 0;
	}

	/**
	 * @brief Returns the argument as type @p T.
	 */
	template<class T>
	T& unsafeGet() const noexcept
	{
		return *static_cast<T*>(m_data);
	}

	/**
	 * @copydoc unsafeGet() const
	 *
	 * @throws Exception if the argument is not of type @p T.
	 */
	template<class T>
	T& get() const
	{
		if (!isOfType<T>())
		{
			throw std::logic_error(
			 neurala::toString("Stored object type \"",
			                   demangle(m_type.name()) ? demangle(m_type.name()).get() : m_type.name(),
			                   " != \"",
			                   typeName<T>(),
			                   '\"'));
		}

		return unsafeGet<T>();
	}
};

} // namespace neurala

#endif // NEURALA_UTILS_DETAIL_ANY_REF_H
