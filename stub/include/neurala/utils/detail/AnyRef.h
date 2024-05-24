/*
 * Copyright Neurala Inc. 2013-2024
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

#ifndef NEURALA_UTILS_DETAIL_ANY_REF_H
#define NEURALA_UTILS_DETAIL_ANY_REF_H

#include <cstring>
#include <type_traits>
#include <typeindex>

#include "neurala/exports.h"
#include "neurala/meta/detail/demangle.h"
#include "neurala/meta/detail/typeName.h"
#include "neurala/utils/string.h"

namespace neurala
{
/**
 * @brief Type-erased reference wrapper.
 *
 * It type erases the argument that it stores, but performs type checking at retrieval to avoid
 * incorrect casts at run-time.
 */
class NEURALA_PUBLIC AnyRef
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
	{ }

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
