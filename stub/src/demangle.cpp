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

#ifndef _MSC_VER
#include <cxxabi.h>
#include <memory>
#endif // !_MSC_VER

#include "neurala/utils/demangle.h"

namespace neurala
{
std::string
demangle(std::string_view name)
{
#ifdef _MSC_VER
	// MSVC returns human-readable names by default
	return std::string{name};
#else
	int status = 0;
	std::unique_ptr<char> result{abi::__cxa_demangle(name.data(), nullptr, nullptr, &status)};

	switch (status)
	{
		// The demangling operation succeeded
		case 0:
			return std::string{result.get()};

		// A memory allocation failure occurred
		case -1:
			throw std::bad_alloc{};

		// `name` is not a valid name under the C++ ABI mangling rules
		case -2:
			return std::string{name};

		// One of the arguments is invalid
		case -3:
		default:
			return {};
	}
#endif // _MSC_VER
}

} // namespace neurala
