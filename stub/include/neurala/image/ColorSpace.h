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
