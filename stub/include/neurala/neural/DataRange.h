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
	 NEURALA_META_ENUM_ENTRY(EDataRange, normalizedScaled),
	 NEURALA_META_ENUM_ENTRY(EDataRange, normalizedCOCO));

	static constexpr const auto fallbackValue = values[0];
};
#endif // SWIG

} // namespace neurala

#endif // NEURALA_NEURAL_DATA_RANGE_H
