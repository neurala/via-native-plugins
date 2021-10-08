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

#ifndef NEURALA_IMAGE_IMAGE_DATA_LAYOUT_H
#define NEURALA_IMAGE_IMAGE_DATA_LAYOUT_H

#include <cstddef>

#include "neurala/meta/enum.h"

namespace neurala
{
/**
 * @brief Image data layout type.
 */
enum class EImageDataLayout : std::uint8_t
{
	unknown = 0x0,
	/// Planar (XXXYYYZZZ)
	planar,
	/// Interleaved (XYZXYZXYZ)
	interleaved,
	/// Semi-planar (XXXYZYZYZ)
	semiplanar
};


// NOTE:20210927:jgerity:SWIG does not support `auto` without a trailing return type declaration (http://www.swig.org/Doc4.0/SWIGDocumentation.html#CPlusPlus11_alternate_function_syntax)
#ifndef SWIG
template<>
class MetaEnum<EImageDataLayout> : public MetaEnumRegister<EImageDataLayout>
{
public:
	static constexpr const auto values = enumRegisterValues(
	 NEURALA_META_ENUM_ENTRY(EImageDataLayout, unknown),
	 NEURALA_META_ENUM_ENTRY(EImageDataLayout, planar),
	 NEURALA_META_ENUM_ENTRY(EImageDataLayout, interleaved),
	 NEURALA_META_ENUM_ENTRY(EImageDataLayout, semiplanar));

	static constexpr const auto fallbackValue = values[0];
};
#endif // SWIG

} // namespace neurala

#endif // NEURALA_IMAGE_IMAGE_DATA_LAYOUT_H
