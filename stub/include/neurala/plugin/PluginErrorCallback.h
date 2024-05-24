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

#ifndef NEURALA_PLUGIN_PLUGIN_ERROR_CALLBACK_H
#define NEURALA_PLUGIN_PLUGIN_ERROR_CALLBACK_H

#include <string>

#include "neurala/error/B4BError.h"
#include "neurala/exports.h"

namespace neurala
{
/**
 * @brief Utility class to transport exception messages between @ref PluginManager and plugins.
 */
class NEURALA_PUBLIC PluginErrorCallback
{
	std::error_code m_code{B4BError::ok()};
	std::string m_exception;

public:
	void operator()(const char* s) noexcept
	{
		try
		{
			m_code = B4BError::genericError();
			m_exception = s;
		}
		catch (...)
		{
			// ignore all exceptions due to string creation
		}
	}

	void operator()(const std::error_code& code, const char* s) noexcept
	{
		try
		{
			m_code = code;
			m_exception = s;
		}
		catch (...)
		{
			// ignore all exceptions due to string creation
		}
	}

	const char* what() const noexcept
	{
		if (m_exception.empty())
		{
			return "failed to propagate error message";
		}
		return m_exception.c_str();
	}

	const std::error_code& code() const noexcept { return m_code; }
};

} // namespace neurala

#endif // NEURALA_PLUGIN_PLUGIN_ERROR_CALLBACK_H
