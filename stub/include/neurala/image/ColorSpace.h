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

#ifndef NEURALA_IMAGE_COLOR_SPACE_H
#define NEURALA_IMAGE_COLOR_SPACE_H

#include <cstdint>

#include "neurala/meta/enum.h"

namespace neurala
{
/**
 * @brief Color space type.
 */
enum class EColorSpace : std::uint8_t
{
	unknown = 0x0,
	/// Single Channel (gray)
	grayscale,
	/// Bayer RGGB
	bayerRG,
	/// Bayer GRBG
	bayerGR,
	/// Bayer BGGR
	bayerBG,
	/// Bayer GBRG
	bayerGB,
	/// Red-Green-Blue (RGB)
	RGB,
	/// Red-Green-Blue-Alpha (RGBA)
	RGBA,
	/// Blue-Green-Red (BGR)
	BGR,
	/// Blue-Green-Red-Alpha (BGRA)
	BGRA,
	/// RGB interleaved, 5bits Red, 6bits Green, 5bits Blue
	RGB565,
	/// Hue-Saturation-Value (HSV)
	HSV,
	/// YUV 4:2:0
	YUV420,
	/// YUV 4:2:0 semi-planar (Y plane, followed by interleaved U, V planes: YYYYYYYY UVUV)
	NV12,
	/// YUV 4:2:0 semi-planar (Y plane, followed by interleaved U, V planes: YYYYYYYY VUVU)
	NV21,
	// YUV 4:2:2
	YUV422
};

#ifndef SWIG
/// @ingroup NEURALA_META_ENUM
template<>
class MetaEnum<EColorSpace> : public MetaEnumRegister<EColorSpace>
{
public:
	static constexpr const auto values = enumRegisterValues(
	 NEURALA_META_ENUM_ENTRY(EColorSpace, unknown),
	 NEURALA_META_ENUM_ENTRY(EColorSpace, grayscale),
	 NEURALA_META_ENUM_ENTRY(EColorSpace, bayerRG),
	 NEURALA_META_ENUM_ENTRY(EColorSpace, bayerGR),
	 NEURALA_META_ENUM_ENTRY(EColorSpace, bayerBG),
	 NEURALA_META_ENUM_ENTRY(EColorSpace, bayerGB),
	 NEURALA_META_ENUM_ENTRY(EColorSpace, RGB),
	 NEURALA_META_ENUM_ENTRY(EColorSpace, RGBA),
	 NEURALA_META_ENUM_ENTRY(EColorSpace, BGR),
	 NEURALA_META_ENUM_ENTRY(EColorSpace, BGRA),
	 NEURALA_META_ENUM_ENTRY(EColorSpace, RGB565),
	 NEURALA_META_ENUM_ENTRY(EColorSpace, HSV),
	 NEURALA_META_ENUM_ENTRY(EColorSpace, YUV420),
	 NEURALA_META_ENUM_ENTRY(EColorSpace, NV12),
	 NEURALA_META_ENUM_ENTRY(EColorSpace, NV21),
	 NEURALA_META_ENUM_ENTRY(EColorSpace, YUV422));

	static constexpr const auto fallbackValue = values[0];
};

#endif // SWIG

/**
 * @brief Returns the number of channels of color space @p colorSpace.
 */
constexpr std::size_t
colorSpaceChannelCount(EColorSpace colorSpace) noexcept
{
	switch (colorSpace)
	{
		case EColorSpace::grayscale:
		case EColorSpace::bayerRG:
		case EColorSpace::bayerGR:
		case EColorSpace::bayerGB:
		case EColorSpace::bayerBG:
			return 1u;
		case EColorSpace::NV12:
		case EColorSpace::NV21:
			return 2u;
		case EColorSpace::RGB:
		case EColorSpace::BGR:
		case EColorSpace::HSV:
		case EColorSpace::RGB565:
		case EColorSpace::YUV420:
		case EColorSpace::YUV422:
			return 3u;
		case EColorSpace::RGBA:
		case EColorSpace::BGRA:
			return 4u;

		default:
			return 0u;
	}
}

} // namespace neurala

#endif // NEURALA_IMAGE_COLOR_SPACE_H
