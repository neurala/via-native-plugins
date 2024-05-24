/*
 * Copyright Neurala Inc. 2013-2024
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

	ImageMetadata() = default;

	/**
	 * @brief Constructs a new @ref ImageMetadata with the provided arguments.
	 *
	 * @param datatype    image data type
	 * @param width       image widht
	 * @param height      image height
	 * @param colorSpace  image color space
	 * @param layout      image data layout
	 * @param orientation image orientation
	 */
	ImageMetadata(std::string datatype,
	              size_type width,
	              size_type height,
	              std::string colorSpace,
	              std::string layout,
	              std::string orientation) noexcept
	 : m_width{width},
	   m_height{height},
	   m_datatype{std::move(datatype)},
	   m_colorSpace{std::move(colorSpace)},
	   m_layout{std::move(layout)},
	   m_orientation{std::move(orientation)}
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
	void width(size_type w) noexcept { m_width = w; }

	/**
	 * @brief Returns the image height in pixels.
	 */
	size_type height() const noexcept { return m_height; }

	/**
	 * @brief Sets the image height in pixels.
	 */
	void height(size_type h) noexcept { m_height = h; }

	/**
	 * @brief Returns the image data type.
	 */
	const std::string& datatype() const noexcept { return m_datatype; }

	/**
	 * @brief Sets the image data layout.
	 */
	void datatype(const std::string& d) noexcept { m_datatype = d; }

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
	 * @brief Returns the image orientation.
	 */
	const std::string& orientation() const noexcept { return m_orientation; }

	/**
	 * @brief Sets the image orientation.
	 */
	void orientation(const std::string& o) noexcept { m_orientation = o; }

// NOTE:20210927:jgerity:SWIG does not support `friend constexpr`
// (https://github.com/swig/swig/issues/2079)
#ifndef SWIG
	friend bool operator==(const ImageMetadata& x, const ImageMetadata& y) noexcept
	{
		return x.width() == y.width() && x.height() == y.height() && x.datatype() == y.datatype()
		       && x.colorSpace() == y.colorSpace() && x.layout() == y.layout()
		       && x.orientation() == y.orientation();
	}

	friend bool operator!=(const ImageMetadata& x, const ImageMetadata& y) noexcept
	{
		return !(x == y);
	}

	friend std::ostream& operator<<(std::ostream& os, const ImageMetadata& metadata)
	{
		return os << metadata.datatype() << ',' << metadata.width() << 'x' << metadata.height() << ','
		          << metadata.colorSpace() << ',' << metadata.layout() << ',' << metadata.orientation();
	}
#endif // SWIG

private:
	size_type m_width{};
	size_type m_height{};
	std::string m_datatype;
	std::string m_colorSpace;
	std::string m_layout;
	std::string m_orientation;
};

} // namespace dto
} // namespace neurala

#endif // NEURALA_IMAGE_DTO_IMAGE_METADATA_H
