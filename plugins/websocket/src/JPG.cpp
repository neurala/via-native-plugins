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

#include <cstddef>
#include <iostream>
#include <system_error>

#include <jpeglib.h>
#include <neurala/video/VideoSourceStatus.h>
#include <websocket/JPG.h>


namespace neurala::plug::ws::jpg
{

namespace
{
/**
 * @brief JPEG Error manager
 */
class JPEGErrorManager
{
private:
	jpeg_error_mgr m_manager;

public:
	JPEGErrorManager() : m_manager{}
	{
		// Setup default error handling
		jpeg_std_error(&m_manager);

		// Called on fatal error, should not return
		// https://github.com/libjpeg-turbo/libjpeg-turbo/blob/master/libjpeg.txt#L1494-L1501
		m_manager.error_exit = +[](j_common_ptr cinfo) {
			// Create message output
			std::array<char, JMSG_LENGTH_MAX> errorMessage{};
			cinfo->err->format_message(cinfo, errorMessage.data());

			// Cleanup jpeg object
			jpeg_destroy(cinfo);

			// Log error
			std::cerr << "Fatal JPEG Library Error: " << errorMessage.data() << '\n';
		};

		/*
		 * Decide whether to emit a trace or warning message.
		 * msg_level is one of:
		 *   -1: recoverable corrupt-data warning, may want to abort.
		 *    0: important advisory messages (always display to user).
		 *    1: first level of tracing detail.
		 *    2,3,...: successively more detailed tracing messages.
		 */
		// https://github.com/libjpeg-turbo/libjpeg-turbo/blob/master/libjpeg.txt#L1515-L1519
		m_manager.emit_message = +[](j_common_ptr cinfo, int msg_level) {
			if (msg_level < 0) // Always show all warning messages (eg. corrupt data)
			{
				// Increment warning counter
				cinfo->err->num_warnings++;

				// Output warning
				std::array<char, JMSG_LENGTH_MAX> errorMessage{};
				cinfo->err->format_message(cinfo, errorMessage.data());
				std::cerr << "Fatal JPEG Library Warning: " << errorMessage.data() << '\n';
			}
			else if (msg_level <= cinfo->err->trace_level) // It's a trace message. Show it if msg_level <=
			                                               // trace_level.
			{
				// Create message output
				std::array<char, JMSG_LENGTH_MAX> errorMessage{};
				cinfo->err->format_message(cinfo, errorMessage.data());

				// Output info or debug message depending on message level
				if (msg_level == 0)
				{
					std::clog << "JPEG: " << errorMessage.data() << '\n';
				}
			}
		};

		// Setup JPEG trace level, all trace messages with msg_level <= trace_level will be logged
		m_manager.trace_level = 1;
	}

	/// @brief A configured jpeg_error_mgr to use with libjpeg
	auto& manager() noexcept { return m_manager; }
};
}

std::pair<neurala::ImageMetadata, std::error_code>
read(const void* const data, const std::size_t size, std::vector<std::byte>& output)
{
	JPEGErrorManager error{};
	jpeg_decompress_struct decompress{};
	decompress.err = &error.manager();
	jpeg_create_decompress(&decompress);

	jpeg_mem_src(&decompress, static_cast<const unsigned char*>(data), size);

	if (!jpeg_read_header(&decompress, TRUE) == JPEG_HEADER_OK)
	{
		std::cerr << "Failed to read JPG header.\n";
		jpeg_destroy_decompress(&decompress);
		return {neurala::ImageMetadata{}, make_error_code(VideoSourceStatus::error)};
	}

	decompress.out_color_space = JCS_RGB;
	if (!jpeg_start_decompress(&decompress))
	{
		std::cerr << "Failed to decompress JPG.\n";
		jpeg_destroy_decompress(&decompress);
		return {neurala::ImageMetadata{}, make_error_code(VideoSourceStatus::error)};
	}

	const auto stride{decompress.out_color_components * decompress.output_width};
	output.resize(stride * decompress.image_height);
	auto* ptr{reinterpret_cast<unsigned char*>(output.data())};
	while (decompress.output_scanline < decompress.output_height)
	{
		jpeg_read_scanlines(&decompress, &ptr, 1);
		ptr += stride;
	}
	jpeg_finish_decompress(&decompress);

	jpeg_destroy_decompress(&decompress);
	return {neurala::ImageMetadata{decompress.output_width,
	                               decompress.output_height,
	                               EColorSpace::RGB,
	                               EImageDataLayout::interleaved,
	                               EDatatype::uint8},
	        make_error_code(VideoSourceStatus::success)};
}

} // namespace neurala::plug::ws::jpg
