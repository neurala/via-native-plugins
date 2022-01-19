/*
 * Copyright Neurala Inc. 2013-2022
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

#ifndef NEURALA_IMAGE_DTO_IMAGE_METADATA_H
#define NEURALA_IMAGE_DTO_IMAGE_METADATA_H

#include <cstddef>
#include <ostream>
#include <string>
#include <utility>

namespace neurala
{
namespace dto
{
/**
 * @brief Image metadata transfer class.
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
	std::string m_colorSpace;
	std::string m_layout;
	std::string m_datatype;

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
	ImageMetadata(size_type width,
	              size_type height,
	              std::string colorSpace,
	              std::string layout,
	              std::string datatype) noexcept
	 : m_width{width},
	   m_height{height},
	   m_colorSpace{std::move(colorSpace)},
	   m_layout{std::move(layout)},
	   m_datatype{std::move(datatype)}
	{ }

	/**
	 * @brief Returns if the image has no pixels.
	 */
	bool empty() const noexcept { return width() == 0 || height() == 0; }

	/**
	 * @brief Returns the image width in pixels.
	 */
	size_type width() const noexcept { return m_width; }

	/**
	 * @brief Sets the image width in pixels.
	 */
	void width(size_type n) noexcept { m_width = n; }

	/**
	 * @brief Returns the image height in pixels.
	 */
	size_type height() const noexcept { return m_height; }

	/**
	 * @brief Sets the image height in pixels.
	 */
	void height(size_type n) noexcept { m_height = n; }

	/**
	 * @brief Returns the image color model.
	 */
	const std::string& colorSpace() const noexcept { return m_colorSpace; }

	/**
	 * @brief Sets the image color model.
	 */
	void colorSpace(const std::string& c) noexcept { m_colorSpace = c; }

	/**
	 * @brief Returns the image data layout.
	 */
	const std::string& layout() const noexcept { return m_layout; }

	/**
	 * @brief Sets the image data layout.
	 */
	void layout(const std::string& l) noexcept { m_layout = l; }

	/**
	 * @brief Returns the image data type.
	 */
	const std::string& datatype() const noexcept { return m_datatype; }

	/**
	 * @brief Sets the image data layout.
	 */
	void datatype(const std::string& d) noexcept { m_datatype = d; }

// NOTE:20210927:jgerity:SWIG does not support `friend constexpr`
// (https://github.com/swig/swig/issues/2079)
#ifndef SWIG
	friend bool operator==(const ImageMetadata& x, const ImageMetadata& y) noexcept
	{
		return x.width() == y.width() && x.height() == y.height() && x.colorSpace() == y.colorSpace()
		       && x.datatype() == y.datatype() && x.layout() == y.layout();
	}

	friend bool operator!=(const ImageMetadata& x, const ImageMetadata& y) noexcept
	{
		return !(x == y);
	}

	friend std::ostream& operator<<(std::ostream& os, const ImageMetadata& metadata)
	{
		return os << metadata.width() << 'x' << metadata.height() << ',' << metadata.colorSpace() << ','
		          << metadata.datatype() << ',' << metadata.layout();
	}
#endif // SWIG
};

} // namespace dto
} // namespace neurala

#endif // NEURALA_IMAGE_DTO_IMAGE_METADATA_H
