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

#ifndef NEURALA_PLUG_WS_JPG_H
#define NEURALA_PLUG_WS_JPG_H

#include <cstddef>
#include <iostream>
#include <system_error>

#include <jpeglib.h>
#include <neurala/image/ImageMetadata.h>
#include <neurala/video/VideoSourceStatus.h>

namespace neurala::plug::ws::jpg
{
inline ImageMetadata
metadata()
{
	// TODO
	return {width, height, EColorSpace::RGB, EImageDataLayout::interleaved, EDatatype::uint8};
}


inline std::error_code
read(const void* const data, const std::size_t size, std::vector<std::byte>& output)
{
	JPEGErrorManager error{};
	jpeg_decompress_struct decompress{};
	decompress.err = &error.manager();
	jpeg_create_decompress(&decompress);
	const auto cleanupDecompress{
	 callOnScopeExit([&decompress] { jpeg_destroy_decompress(&decompress); })};
	jpeg_mem_src(&decompress, data, size);
	auto orientation = imageOrientation(data, size);
	if (imageOrientation(data, size) != EImageOrientation::topLeft)
	{
		std::cerr << "The only supported Exif orientation is top-left.\n";
		return make_error_code(VideoSourceStatus::error);
	}
	if (!jpeg_read_header(&decompress, TRUE) == JPEG_HEADER_OK)
	{
		std::cerr << "Failed to read JPG header.\n";
		return make_error_code(VideoSourceStatus::error);
	}
	decompress.out_color_space = JCS_RGB;
	if (!jpeg_start_decompress(&decompress))
	{
		std::cerr << "Failed to decompress JPG.\n";
		return make_error_code(VideoSourceStatus::error);
	}
	const auto stride{decompress.out_color_components * decompress.output_width};
	output.resize(stride * decompress.image_height);
	std::uint8_t* ptr{output.data()};
	while (decompress.output_scanline < decompress.output_height)
	{
		jpeg_read_scanlines(&decompress, &ptr, 1);
		ptr += stride;
	}
	jpeg_finish_decompress(&decompress);
	return make_error_code(VideoSourceStatus::success);
}

} // namespace neurala::plug::ws::jpg

#endif // NEURALA_PLUG_WS_JPG_H
