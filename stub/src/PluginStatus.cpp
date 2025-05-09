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

#include "neurala/error/B4BError.h"

#include "neurala/plugin/PluginStatus.h"

namespace
{
/**
 * @brief Returns a string representation of @p status.
 */
const char*
pluginStatusToStr(neurala::PluginStatus status)
{
	switch (status)
	{
		case neurala::PluginStatus::success():
			return "success";
		case neurala::PluginStatus::wrongVersion():
			return "incompatible version";
		case neurala::PluginStatus::invalidName():
			return "invalid name";
		case neurala::PluginStatus::alreadyRegistered():
			return "already registered";
		case neurala::PluginStatus::unknown():
		default:
			return "unknown";
	}
}

class PluginStatusCategory : public std::error_category
{
public:
	const char* name() const noexcept override { return "neurala::PluginStatus"; }

	std::string message(int c) const override { return pluginStatusToStr({c}); }

	bool equivalent(int code, const std::error_condition& condition) const noexcept override
	{
		// Avoid infinite loops
		if (*this == condition.category())
		{
			return code == condition.value();
		}

		switch (code)
		{
			case neurala::PluginStatus::success():
				return neurala::B4BError::ok() == condition;
			case neurala::PluginStatus::wrongVersion():
			case neurala::PluginStatus::invalidName():
				return neurala::B4BError::genericError() == condition;
			case neurala::PluginStatus::unknown():
				return neurala::B4BError::unknown() == condition;
			default:
				return false;
		}
	}

	bool equivalent(const std::error_code& code, int condition) const noexcept override
	{
		// Avoid infinite loops
		if (code.category() == *this)
		{
			return code.value() == condition;
		}

		switch (condition)
		{
			case neurala::PluginStatus::success():
				return code == neurala::B4BError::ok();
			case neurala::PluginStatus::wrongVersion():
			case neurala::PluginStatus::invalidName():
				return code == neurala::B4BError::genericError();
			case neurala::PluginStatus::unknown():
				return code == neurala::B4BError::unknown();
			default:
				return false;
		}
	}
};
} // namespace

namespace neurala
{
const std::error_category&
pluginStatusCategory() noexcept
{
	static const PluginStatusCategory category;
	return category;
}

std::error_condition
make_error_condition(PluginStatus status) noexcept // NOLINT
{
	return std::error_condition{static_cast<int>(status), pluginStatusCategory()};
}

std::error_code
make_error_code(PluginStatus status) noexcept // NOLINT
{
	return std::error_code{static_cast<int>(status), pluginStatusCategory()};
}
} // namespace neurala
