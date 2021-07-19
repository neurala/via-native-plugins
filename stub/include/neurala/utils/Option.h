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

#ifndef NEURALA_UTILS_OPTION_H
#define NEURALA_UTILS_OPTION_H

#include <cstdint>
#include <memory>
#include <ostream>
#include <string>
#include <type_traits>
#include <utility>

#include "neurala/exports.h"

namespace neurala
{
/**
 * @brief User-defined option container.
 */
class NEURALA_PUBLIC Option
{
public:
	using size_type = std::size_t;

private:
	class Impl;

	std::unique_ptr<Impl> m_impl;

public:
	Option();

	/**
	 * @brief Constructs an option that maps key @p key to value @p value.
	 *
	 * @param key   option key
	 * @param value option value
	 */
	template<class T,
	         std::enable_if_t<std::is_integral<T>::value && !std::is_same<std::decay_t<T>, bool>::value, bool> = false>
	Option(const std::string& key, const T& value) : Option()
	{
		add(key, std::int64_t(value));
	}

	/// @copydoc Option(const std::string&,const T&)
	template<class T, std::enable_if_t<std::is_same<std::decay_t<T>, bool>::value, bool> = true>
	Option(const std::string& key, const T& value) : Option()
	{
		add(key, bool(value));
	}

	/// @copydoc Option(const std::string&,const T&)
	Option(const std::string& key, double value);

	/// @copydoc Option(const std::string&,const T&)
	Option(const std::string& key, const std::string& value);

	Option(const Option&);

	Option(Option&&) noexcept;

	~Option();

	Option& operator=(const Option&);

	Option& operator=(Option&&) noexcept;

	/**
	 * @brief Returns if there are no options stored.
	 */
	bool empty() const noexcept;

	/**
	 * @brief Returns the number of stored options.
	 */
	size_type size() const noexcept;

	/**
	 * @brief Inserts an option that maps key @p key to value @p value.
	 *
	 * @warning If there is an option with the same key, it will be overwritten.
	 *
	 * @param key   option key
	 * @param value option value
	 */
	Option& add(const std::string& key, bool value);

	/// @copydoc add(const std::string&,bool)
	Option& add(const std::string& key, std::int64_t value);

	/// @copydoc add(const std::string&,bool)
	Option& add(const std::string& key, double value);

	/// @copydoc add(const std::string&,bool)
	Option& add(const std::string& key, const std::string& value);

	/**
	 * @brief Returns the option associated with key @p key as a @c bool.
	 */
	bool asBool(const std::string& key) const;

	/**
	 * @brief Returns the option associated with key @p key as a @c std::int64_t.
	 */
	std::int64_t asInt(const std::string& key) const;

	/**
	 * @brief Returns the option associated with key @p key as a @c double.
	 */
	double asDouble(const std::string& key) const;

	/**
	 * @brief Returns the option associated with key @p key as a @c std::string.
	 */
	const std::string& asString(const std::string& key) const;

	/**
	 * @brief Returns the option associated with key @p key as a @c bool or @c nullptr if it does
	 *        not exist or is the wrong type.
	 */
	const bool* tryAsBool(const std::string& key) const;

	/**
	 * @brief Returns the option associated with key @p key as a @c std::int64_t or @c nullptr if it
	 *        does not exist or is the wrong type.
	 */
	const std::int64_t* tryAsInt(const std::string& key) const;

	/**
	 * @brief Returns the option associated with key @p key as a @c double or @c nullptr if it does
	 *        not exist or is the wrong type.
	 */
	const double* tryAsDouble(const std::string& key) const;

	/**
	 * @brief Returns the option associated with key @p key as a @c std::string or @c nullptr if it
	 *        does not exist or is the wrong type.
	 */
	const std::string* tryAsString(const std::string& key) const;

	/**
	 * @brief Merges the options in @p other into this object.
	 *
	 * @warning Options in @p other with the same key as in @p this will overwrite the latter.
	 */
	Option& mergeIn(const Option& other);

	/// @copydoc mergeIn(const Option&)
	Option& mergeIn(Option&& other);

	/// @copydoc mergeIn(const Option&)
	Option& operator|=(const Option& other);

	/// @copydoc mergeIn(const Option&)
	Option& operator|=(Option&& other);

	/**
	 * @brief Creates a new @ref Option object by merging @p x and @p y.
	 *
	 * @warning Options in @p other with the same key as in @p this will overwrite the latter.
	 */
	friend NEURALA_PUBLIC Option operator|(Option x, const Option& y);

	/// @copydoc operator|(Option,const Option&)
	friend NEURALA_PUBLIC Option operator|(Option x, Option&& y);

	friend NEURALA_PUBLIC std::ostream& operator<<(std::ostream& os, const Option& option);
};

} // namespace neurala

#endif // NEURALA_UTILS_OPTION_H
