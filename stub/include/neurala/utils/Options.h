/*
 * Copyright Neurala Inc. 2013-2025
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
class NEURALA_PUBLIC Options
{
public:
	using size_type = std::size_t;

private:
	class Impl;
	struct ImplDelete
	{
		void operator()(Impl*) const;
	};

	std::unique_ptr<Impl, ImplDelete> m_impl;

	template<typename T>
	T as(const std::string& key, const T& defaultValue) const;

public:
	Options();

// NOTE:2022-05-16:jgerity:Type trait usage here is unsupported by SWIG
#ifndef SWIG
	/**
	 * @brief Constructs an option that maps key @p key to value @p value.
	 *
	 * @param key   option key
	 * @param value option value
	 */
	template<class T,
	         std::enable_if_t<std::is_integral<T>::value && !std::is_same<std::decay_t<T>, bool>::value,
	                          bool> = false>
	Options(const std::string& key, const T& value) : Options()
	{
		add(key, std::int64_t(value));
	}
#endif // SWIG

	/// @copydoc Options(const std::string&,const T&)
	template<class T, std::enable_if_t<std::is_same<std::decay_t<T>, bool>::value, bool> = true>
	Options(const std::string& key, const T& value) : Options()
	{
		add(key, bool(value));
	}

	/// @copydoc Options(const std::string&,const T&)
	Options(const std::string& key, double value);

	/// @copydoc Options(const std::string&,const T&)
	Options(const std::string& key, const std::string& value);

	Options(const Options&);
	Options(Options&&) = default;
	Options& operator=(const Options&);
	Options& operator=(Options&&) = default;
	~Options() = default;

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
	Options& add(const std::string& key, bool value);

	/// @copydoc add(const std::string&,bool)
	Options& add(const std::string& key, std::int64_t value);

	/// @copydoc add(const std::string&,bool)
	Options& add(const std::string& key, double value);

	/// @copydoc add(const std::string&,bool)
	Options& add(const std::string& key, const std::string& value);

	/// @copydoc add(const std::string&,bool)
	Options& add(const std::string& key, const char* value);

	/**
	 * @brief Returns the option associated with key @p key as a @c bool.
	 */
	bool asBool(const std::string& key) const;

	/**
	 * @brief Returns the option associated with key @p key.
	 * if it does not exist returns @p defaultValue
	 */
	bool asBool(const std::string& key, bool defaultValue) const;

	/// @copydoc asBool(const std::string&, bool)
	std::int64_t asInt(const std::string& key, std::int64_t defaultValue) const;

	/// @copydoc asBool(const std::string&, bool)
	double asDouble(const std::string& key, double defaultValue) const;

	/// @copydoc asBool(const std::string&, bool)
	std::string asString(const std::string& key, const std::string& defaultValue) const;

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
	Options& mergeIn(const Options& other);

	/// @copydoc mergeIn(const Options&)
	Options& mergeIn(Options&& other);

	/// @copydoc mergeIn(const Options&)
	Options& operator|=(const Options& other);

	/// @copydoc mergeIn(const Options&)
	Options& operator|=(Options&& other);

	/**
	 * @brief Creates a new @ref Options object by merging @p x and @p y.
	 *
	 * @warning Options in @p other with the same key as in @p this will overwrite the latter.
	 */
	friend NEURALA_PUBLIC Options operator|(Options x, const Options& y);

	/// @copydoc operator|(Options,const Options&)
	friend NEURALA_PUBLIC Options operator|(Options x, Options&& y);

	friend NEURALA_PUBLIC std::ostream& operator<<(std::ostream& os, const Options& option);
};

} // namespace neurala

#endif // NEURALA_UTILS_OPTION_H
