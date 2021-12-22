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

#include "neurala/error/B4BError.h"

namespace neurala
{
namespace
{
class B4BCategoryImpl final : public std::error_category
{
public:
	virtual const char* name() const noexcept { return "b4b_error"; }
	virtual std::string message(int ev) const noexcept
	{
		switch (ev)
		{
			case B4BError::ok():
				return "ok";
			case B4BError::notImplemented():
				return "notImplemented";
			case B4BError::genericError():
				return "genericError";
			case B4BError::invalidParameter():
				return "invalidParameter";
			case B4BError::unsupportedAction():
				return "unsupportedAction";
			case B4BError::unknown():
			default:
				return "unknown";
		}
	}
};
} // namespace

const std::error_category&
b4bCategory() noexcept
{
	static B4BCategoryImpl category;
	return category;
}

std::error_code
make_error_code(B4BError error) noexcept // NOLINT
{
	return std::error_code{static_cast<int>(error), b4bCategory()};
}

std::error_condition
make_error_condition(B4BError error) noexcept // NOLINT
{
	return std::error_condition{static_cast<int>(error), b4bCategory()};
}
} // namespace neurala
