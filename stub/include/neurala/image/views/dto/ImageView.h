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

#ifndef NEURALA_IMAGE_VIEWS_DTO_IMAGE_VIEW_H
#define NEURALA_IMAGE_VIEWS_DTO_IMAGE_VIEW_H

#include "neurala/image/dto/ImageMetadata.h"

namespace neurala
{
namespace dto
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

	/// @copydoc ImageMetadata::empty() const
	constexpr bool empty() const noexcept { return m_metadata.empty(); }

	/// @copydoc ImageMetadata::width() const
	constexpr size_type width() const noexcept { return m_metadata.width(); }

	/// @copydoc ImageMetadata::height() const
	constexpr size_type height() const noexcept { return m_metadata.height(); }

	/// @copydoc ImageMetadata::channels() const
	constexpr size_type channels() const noexcept { return m_metadata.channels(); }

	/// @copydoc ImageMetadata::pixelComponentCount() const
	constexpr size_type pixelComponentCount() const noexcept
	{
		return m_metadata.pixelComponentCount();
	}

	/// @copydoc ImageMetadata::pixelCount() const
	constexpr size_type pixelCount() const noexcept { return m_metadata.pixelCount(); }

	/// @copydoc ImageMetadata::colorSpace() const
	constexpr EColorSpace colorSpace() const noexcept { return m_metadata.colorSpace(); }

	/// @copydoc ImageMetadata::layout() const
	constexpr EImageDataLayout layout() const noexcept { return m_metadata.layout(); }

	/// @copydoc ImageMetadata::datatype() const
	constexpr EDatatype datatype() const noexcept { return m_metadata.datatype(); }

	/// @copydoc ImageMetadata::sizeBytes() const
	constexpr size_type sizeBytes() const noexcept { return m_metadata.sizeBytes(); }

	/**
	 * @brief Returns the metadata associated with this view.
	 */
	constexpr const ImageMetadata& metadata() const noexcept { return m_metadata; }

	/**
	 * @brief Returns the data associated with this view.
	 */
	constexpr const void* data() const noexcept { return m_data; }

	/**
	 * @brief Returns the data associated with this view as @p T.
	 */
	template<class T>
	constexpr const T* dataAs() const noexcept
	{
		return static_cast<const T*>(data());
	}
};

} // namespace dto
} // namespace neurala

#endif // NEURALA_IMAGE_VIEWS_DTO_IMAGE_VIEW_H
