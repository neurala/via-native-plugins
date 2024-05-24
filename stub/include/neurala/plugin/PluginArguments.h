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

#ifndef NEURALA_PLUGIN_PLUGIN_ARGUMENTS_H
#define NEURALA_PLUGIN_PLUGIN_ARGUMENTS_H

#include <utility>
#include <vector>

#include "neurala/exports.h"
#include "neurala/utils/detail/AnyRef.h"

namespace neurala
{
/**
 * @brief Arguments for plugin instance creation.
 *
 * It type erases all arguments passed to it, but performs type checking at retrieval to avoid
 * incorrect casts.
 */
class NEURALA_PUBLIC PluginArguments
{
	std::vector<AnyRef> m_args;

public:
	/**
	 * @brief Creates a new @ref PluginArguments object that type-erases @p t.
	 */
	template<class... T>
	explicit PluginArguments(T&... t) : m_args({t...})
	{ }

	/**
	 * @brief Returns if this object has no arguments.
	 */
	bool empty() const noexcept { return m_args.empty(); }

	/**
	 * @brief Returns the number of arguments stored in this object.
	 */
	std::size_t size() const noexcept { return m_args.size(); }

	/**
	 * @brief Returns if the @p I -th argument is of type @p T.
	 */
	template<std::size_t I, class T>
	bool isOfType() const
	{
		return m_args.at(I).isOfType<T>();
	}

	/**
	 * @brief Returns the @p I -th argument, cast to type @p T.
	 */
	template<std::size_t I, class T>
	T& get() const
	{
		return m_args.at(I).get<T>();
	}
};

} // namespace neurala

#endif // NEURALA_PLUGIN_PLUGIN_ARGUMENTS_H
