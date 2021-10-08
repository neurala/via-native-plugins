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

#ifndef NEURALA_IMAGE_IMAGE_METADATA_H
#define NEURALA_IMAGE_IMAGE_METADATA_H

#include <cstddef>
#include <ostream>

#include "neurala/image/ColorSpace.h"
#include "neurala/image/ImageDataLayout.h"
#include "neurala/meta/Datatype.h"
#include "neurala/neural/DataRange.h"

namespace neurala
{
/**
 * @brief Image metadata class.
 *
 * Describes an image using dimensions, color space, data layout and data type.
 */
class ImageMetadata
{
public:
	using size_type = std::size_t;

private:
	size_type m_width{};
	size_type m_height{};
	EColorSpace m_colorSpace{EColorSpace::unknown};
	EImageDataLayout m_layout{EImageDataLayout::unknown};
	EDatatype m_datatype{EDatatype::unknown};
	EDataRange m_dataRange{EDataRange::unknown};

public:
	ImageMetadata() = default;

	/**
	 * @brief Constructs a new @ref ImageMetadata with the provided arguments.
	 *
	 * @param width      image widht
	 * @param height     image height
	 * @param colorSpace image color space
	 * @param layout     image data layout
	 * @param datatype   image data type
	 */
	constexpr ImageMetadata(size_type width,
	                        size_type height,
	                        EColorSpace colorSpace,
	                        EImageDataLayout layout,
	                        EDatatype datatype) noexcept
	 : m_width{width}, m_height{height}, m_colorSpace{colorSpace}, m_layout{layout}, m_datatype{datatype}
	{
		switch (m_datatype)
		{
			case EDatatype::uint8:
			case EDatatype::uint16:
				m_dataRange = EDataRange::normalizedScaled;
				return;
			case EDatatype::binary32:
			case EDatatype::binary64:
				m_dataRange = EDataRange::normalized;
				return;
			default:
				m_dataRange = EDataRange::unknown;
		}
	}

	/**
	 * @brief Returns if the image has no pixels.
	 */
	constexpr bool empty() const noexcept
	{
		return width() == 0u || height() == 0u || channels() == 0u;
	}

	/**
	 * @brief Returns the image width in pixels.
	 */
	constexpr size_type width() const noexcept { return m_width; }

	/**
	 * @brief Sets the image width in pixels.
	 */
	constexpr void width(size_type n) noexcept { m_width = n; }

	/**
	 * @brief Returns the image height in pixels.
	 */
	constexpr size_type height() const noexcept { return m_height; }

	/**
	 * @brief Sets the image height in pixels.
	 */
	constexpr void height(size_type n) noexcept { m_height = n; }

	/**
	 * @brief Returns the image color model.
	 */
	constexpr EColorSpace colorSpace() const noexcept { return m_colorSpace; }

	/**
	 * @brief Sets the image color model.
	 */
	constexpr void colorSpace(EColorSpace c) noexcept { m_colorSpace = c; }

	/**
	 * @brief Returns the image data layout.
	 */
	constexpr EImageDataLayout layout() const noexcept { return m_layout; }

	/**
	 * @brief Sets the image data layout.
	 */
	constexpr void layout(EImageDataLayout l) noexcept { m_layout = l; }

	/**
	 * @brief Returns the image data type.
	 */
	constexpr EDatatype datatype() const noexcept { return m_datatype; }

	/**
	 * @brief Sets the image data layout.
	 */
	constexpr void datatype(EDatatype d) noexcept { m_datatype = d; }

	/**
	 * @brief Returns the image data range.
	 */
	constexpr EDataRange datarange() const noexcept { return m_dataRange; }

	/**
	 * @brief Sets the image data range.
	 */
	constexpr void datarange(EDataRange d) noexcept { m_dataRange = d; }

	/**
	 * @brief Returns the number of channels.
	 */
	constexpr size_type channels() const noexcept { return colorSpaceChannelCount(m_colorSpace); }

	/**
	 * @brief Returns the number of pixel components in this image.
	 */
	constexpr size_type pixelComponentCount() const noexcept { return pixelCount() * channels(); }

	/**
	 * @brief Returns the number of pixels in this image.
	 */
	constexpr size_type pixelCount() const noexcept { return width() * height(); }

	/**
	 * @brief Returns the number of bytes in this image.
	 */
	constexpr size_type sizeBytes() const noexcept
	{
		return pixelComponentCount() * sizeofDatatype(datatype());
	}

// NOTE:20210927:jgerity:SWIG does not support `friend constexpr` (https://github.com/swig/swig/issues/2079)
#ifndef SWIG
	friend constexpr bool operator==(const ImageMetadata& x, const ImageMetadata& y) noexcept
	{
		return x.width() == y.width() && x.height() == y.height() && x.colorSpace() == y.colorSpace()
		       && x.datatype() == y.datatype() && x.datarange() == y.datarange()
		       && x.layout() == y.layout();
	}

	friend constexpr bool operator!=(const ImageMetadata& x, const ImageMetadata& y) noexcept
	{
		return !(x == y);
	}

	friend std::ostream& operator<<(std::ostream& os, const ImageMetadata& metadata)
	{
		return os << metadata.width() << 'x' << metadata.height() << ',' << metadata.colorSpace() << ','
		          << metadata.datatype() << ',' << metadata.datarange() << ',' << metadata.layout();
	}
#endif // SWIG
};

} // namespace neurala

#endif // NEURALA_IMAGE_IMAGE_METADATA_H
