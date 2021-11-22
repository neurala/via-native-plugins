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

#ifndef NEURALA_NEURAL_DATA_RANGE_H
#define NEURALA_NEURAL_DATA_RANGE_H

#include "neurala/meta/enum.h"

namespace neurala
{
/**
 * @brief Available data preprocessing for neural network input.
 */
enum class EDataRange
{
	/// Unknown data range
	unknown,
	/// Normalized data range <tt>[0.0, 1.0]</tt>
	normalized,
	/// Normalized data range around zero <tt>[-0.5, 0.5]</tt>
	normalizedAroundZero,
	/// Normalized data range around zero <tt>[-1, 1]</tt>
	normalizedAroundZeroSSD,
	/// Scaled data range by 255 <tt>[0.0, 255.0]</tt>
	normalizedScaled,
	/// normalized rgb data based on COCO mean values: channel = (channel - channelMean)/channelStd
	normalizedCOCO
};

// NOTE:20210927:jgerity:SWIG does not support `auto` without a trailing return type declaration (http://www.swig.org/Doc4.0/SWIGDocumentation.html#CPlusPlus11_alternate_function_syntax)
#ifndef SWIG
template<>
class MetaEnum<EDataRange> : public MetaEnumRegister<EDataRange>
{
public:
	static constexpr const auto values = enumRegisterValues(
	 NEURALA_META_ENUM_ENTRY(EDataRange, unknown),
	 NEURALA_META_ENUM_ENTRY(EDataRange, normalized),
	 NEURALA_META_ENUM_ENTRY(EDataRange, normalizedAroundZero),
	 NEURALA_META_ENUM_ENTRY(EDataRange, normalizedAroundZeroSSD),
	 NEURALA_META_ENUM_ENTRY(EDataRange, normalizedScaled),
	 NEURALA_META_ENUM_ENTRY(EDataRange, normalizedCOCO));

	static constexpr const auto fallbackValue = values[0];
};
#endif // SWIG

} // namespace neurala

#endif // NEURALA_NEURAL_DATA_RANGE_H
