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

#ifndef NEURALA_IMAGE_VIEWS_IMAGE_VIEW_H
#define NEURALA_IMAGE_VIEWS_IMAGE_VIEW_H

#include "neurala/algorithms/math/utils.h"
#include "neurala/image/ImageMetadata.h"

namespace neurala
{
/**
 * @brief View over an image.
 *
 * @warning It is the user's responsibility that the data given to the view remains valid for the
 *          lifetime of the view.
 */
class ImageView
{
public:
	using size_type = ImageMetadata::size_type;

private:
	const void* m_data{};
	ImageMetadata m_metadata;

public:
	ImageView() = default;

	/**
	 * @brief Constructs a new @ref ImageView with the provided arguments.
	 *
	 * The data type is derived from the type of @p data.
	 *
	 * @warning It is the user's responsibility to give a pointer to adequate data as described by
	 *          the rest of the parameters.
	 *
	 * @param metadata image metadata
	 * @param data     image data
	 */
	constexpr ImageView(const ImageMetadata& metadata, const void* data) noexcept
	 : m_data{data}, m_metadata{metadata}
	{ }

	/**
	 * @brief Constructs a new @ref ImageView with the provided arguments.
	 *
	 * The data type is derived from the type of @p data.
	 *
	 * @warning It is the user's responsibility to give a pointer to adequate data as described by
	 *          the rest of the parameters.
	 *
	 * @param width      image width
	 * @param height     image height
	 * @param colorSpace image color space
	 * @param layout     image data layout
	 * @param data       image data
	 */
	template<class T>
	constexpr ImageView(size_type width,
	                    size_type height,
	                    EColorSpace colorSpace,
	                    EImageDataLayout layout,
	                    const T* data) noexcept
	 : m_data{data}, m_metadata{width, height, colorSpace, layout, DatatypeTraits<T>::datatype}
	{ }

	virtual ~ImageView() = default;

	/// @brief Returns whether the view contains any pixels.
	virtual bool empty() const noexcept { return emptyImage(); }

	/// @copydoc ImageMetadata::empty() const
	constexpr bool emptyImage() const noexcept { return m_metadata.empty(); }

	/// @brief Returns the viewed width in pixels.
	virtual std::size_t width() const noexcept { return imageWidth(); }

	/// @copydoc ImageMetadata::width() const
	constexpr size_type imageWidth() const noexcept { return m_metadata.width(); }

	/// @brief Returns the viewed height in pixels.
	virtual std::size_t height() const noexcept { return imageHeight(); }

	/// @copydoc ImageMetadata::height() const
	constexpr size_type imageHeight() const noexcept { return m_metadata.height(); }

	/// @brief Returns the number of channels.
	virtual std::size_t channels() const noexcept { return imageChannels(); }

	/// @copydoc ImageMetadata::channels() const
	constexpr size_type imageChannels() const noexcept { return m_metadata.channels(); }

	/// @brief Returns the number of viewed pixels.
	virtual std::size_t pixelCount() const noexcept { return imagePixelCount(); }

	/// @copydoc ImageMetadata::pixelCount() const
	constexpr size_type imagePixelCount() const noexcept { return m_metadata.pixelCount(); }

	/// @brief Returns the number of viewed pixel components.
	virtual std::size_t pixelComponentCount() const noexcept { return imagePixelComponentCount(); }

	/// @copydoc ImageMetadata::pixelComponentCount() const
	constexpr size_type imagePixelComponentCount() const noexcept
	{
		return m_metadata.pixelComponentCount();
	}

	/// @brief Returns the number of viewed bytes.
	virtual std::size_t sizeBytes() const noexcept { return imageSizeBytes(); }

	/// @copydoc ImageMetadata::sizeBytes() const
	constexpr size_type imageSizeBytes() const noexcept { return m_metadata.sizeBytes(); }

	/// @copydoc ImageMetadata::colorSpace() const
	constexpr EColorSpace colorSpace() const noexcept { return m_metadata.colorSpace(); }

	/// @copydoc ImageMetadata::layout() const
	constexpr EImageDataLayout layout() const noexcept { return m_metadata.layout(); }

	/// @copydoc ImageMetadata::datatype() const
	constexpr EDatatype datatype() const noexcept { return m_metadata.datatype(); }

	/// @brief Returns the type of view strategy used.
	virtual std::string viewStrategy() const noexcept { return "identity"; }

	/// @brief Returns the metadata associated with the viewed image.
	constexpr const ImageMetadata& imageMetadata() const noexcept { return m_metadata; }

	/// @brief Returns the data associated with the viewed image.
	constexpr const void* imageData() const noexcept { return m_data; }

	/// @brief Returns the data associated with the viewed image as @p T.
	template<class T>
	constexpr const T* imageDataAs() const noexcept
	{
		return static_cast<const T*>(imageData());
	}

	friend bool operator==(const ImageView& x, const ImageView& y) noexcept
	{
		// choose the correct implementation for non-trivial views
		return x.viewStrategy() == "identity" ? y.isEqualTo(x) : x.isEqualTo(y);
	}

	friend bool operator!=(const ImageView& x, const ImageView& y) noexcept { return !(x == y); }

private:
	virtual bool isEqualTo(const ImageView& im) const noexcept
	{
		if (imageMetadata() != im.imageMetadata())
		{
			return false;
		}
		if (imageData() == im.imageData())
		{
			return true;
		}
		switch (datatype())
		{
			case EDatatype::binary32:
			{
				const auto thisData = static_cast<const float*>(imageData());
				const auto imData = static_cast<const float*>(im.imageData());
				return approxEqual(imData, imData + pixelComponentCount(), imData);
			}
			case EDatatype::binary64:
			{
				const auto thisData = static_cast<const double*>(imageData());
				const auto imData = static_cast<const double*>(im.imageData());
				return approxEqual(thisData, thisData + pixelComponentCount(), imData);
			}
			case EDatatype::uint8:
			{
				const auto thisData = static_cast<const std::uint8_t*>(imageData());
				const auto imData = static_cast<const std::uint8_t*>(im.imageData());
				return approxEqual(thisData, thisData + pixelComponentCount(), imData);
			}
			case EDatatype::boolean:
			{
				const auto thisData = static_cast<const bool*>(imageData());
				const auto imData = static_cast<const bool*>(im.imageData());
				return approxEqual(thisData, thisData + pixelComponentCount(), imData);
			}
			default:
				return false;
		}
	}
};

constexpr const ImageView&
makeImageView(const ImageView& im) noexcept
{
	return im;
}

} // namespace neurala

#endif // NEURALA_IMAGE_VIEWS_IMAGE_VIEW_H
