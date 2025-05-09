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

#ifndef NEURALA_IMAGE_VIEWS_DTO_IMAGE_VIEW_H
#define NEURALA_IMAGE_VIEWS_DTO_IMAGE_VIEW_H

#include <string>

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
	ImageView(const ImageMetadata& metadata, const void* data) noexcept
	 : m_metadata{metadata}, m_data{data}
	{ }

	/// @copydoc ImageMetadata::empty() const
	size_type empty() const noexcept { return m_metadata.empty(); }

	/// @copydoc ImageMetadata::width() const
	size_type width() const noexcept { return m_metadata.width(); }

	/// @copydoc ImageMetadata::height() const
	size_type height() const noexcept { return m_metadata.height(); }

	/// @copydoc ImageMetadata::datatype() const
	const std::string& datatype() const noexcept { return m_metadata.datatype(); }

	/// @copydoc ImageMetadata::colorSpace() const
	const std::string& colorSpace() const noexcept { return m_metadata.colorSpace(); }

	/// @copydoc ImageMetadata::layout() const
	const std::string& layout() const noexcept { return m_metadata.layout(); }

	/// @copydoc ImageMetadata::orientation() const
	const std::string& orientation() const noexcept { return m_metadata.orientation(); }

	/**
	 * @brief Returns the metadata associated with this view.
	 */
	const ImageMetadata& metadata() const noexcept { return m_metadata; }

	/**
	 * @brief Returns the data associated with this view.
	 */
	const void* data() const noexcept { return m_data; }

	/**
	 * @brief Returns the data associated with this view as @p T.
	 */
	template<class T>
	const T* dataAs() const noexcept
	{
		return static_cast<const T*>(data());
	}

private:
	ImageMetadata m_metadata;
	const void* m_data{};
};

} // namespace dto
} // namespace neurala

#endif // NEURALA_IMAGE_VIEWS_DTO_IMAGE_VIEW_H
