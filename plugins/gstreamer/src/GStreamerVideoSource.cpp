/*
 * Copyright Neurala Inc. 2013-2023
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

#include <charconv>
#include <cstdlib>

#include <gst/gst.h>
#include <gst/app/gstappsink.h>

#include "GStreamerVideoSource.h"

namespace neurala
{
namespace
{
class Sample
{
	GstSample* sample;
	GstBuffer* buffer;
	GstMapInfo map;

public:
	explicit Sample(GstSample* sample) noexcept
	 : sample(sample), buffer(gst_sample_get_buffer(sample))
	{
		gst_buffer_map(buffer, &map, GST_MAP_READ);
	}

	~Sample() noexcept
	{
		gst_buffer_unmap(buffer, &map);
		gst_sample_unref(sample);
	}

	dto::ImageView imageView(std::size_t width, std::size_t height) const noexcept
	{
		return dto::ImageView(dto::ImageMetadata("uint8", width, height, "RGB", "interleaved", "topLeft"), map.data);
	}

	Sample(const Sample&) = delete;
	Sample(Sample&&) = delete;

	Sample& operator=(const Sample&) = delete;
	Sample& operator=(Sample&&) = delete;
};
}

struct GStreamerVideoSource::Implementation
{
	GstElement* pipeline;
	GstElement* userPipeline;
	GstElement* sink;

	std::unique_ptr<Sample> sample;
};

void*
GStreamerVideoSource::create(PluginArguments&, PluginErrorCallback&)
{
	if(!gst_is_initialized())
	{
		gst_init(0, NULL);
	}
	return new GStreamerVideoSource("PLACEHOLDER");
}

void
GStreamerVideoSource::destroy(void* p)
{
	delete static_cast<GStreamerVideoSource*>(p);
}

GStreamerVideoSource::GStreamerVideoSource(const char* name)
 : m_implementation(std::make_unique<Implementation>()),
   m_bufferReady(false),
   m_frameReady(false),
   m_endOfStream(false)
{
	m_implementation->pipeline = gst_pipeline_new(name);

	{
		const auto parser = gst_parse_context_new();
		const auto userPipeline = getenv("NEURALA_GSTREAMER_PIPELINE");

		if (!userPipeline)
		{
			m_lastError = B4BError::invalidParameter();
			return;
		}

		m_implementation->userPipeline = gst_parse_launch(userPipeline, nullptr);

		gst_parse_context_free(parser);
	}

	{
		long width;
		long height;

		const auto widthOption = getenv("NEURALA_GSTREAMER_WIDTH");
		const auto heightOption = getenv("NEURALA_GSTREAMER_HEIGHT");

		if (!widthOption || !heightOption)
		{
			m_lastError = B4BError::invalidParameter();
			return;
		}

		static constexpr auto ok = std::errc();

		if (std::from_chars(widthOption, widthOption + strlen(widthOption), width).ec != ok
		    || std::from_chars(heightOption, heightOption + strlen(heightOption), height).ec != ok)
		{
			m_lastError = B4BError::invalidParameter();
			return;
		}

		m_width = width;
		m_height = height;
	}

	{
		const auto sinkName = getenv("NEURALA_GSTREAMER_SINK");

		m_implementation->sink = gst_bin_get_by_name(GST_BIN(m_implementation->userPipeline), sinkName);

		if (!m_implementation->sink)
		{
			m_lastError = B4BError::genericError();
			return;
		}
	}

#ifdef false
	const auto caps = gst_caps_new_simple("video/x-raw", "format", G_TYPE_STRING, "BGR", NULL);

	gst_bin_add_many(GST_BIN(m_implementation->pipeline), m_implementation->userPipeline, m_implementation->sink, nullptr);
	gst_app_sink_set_emit_signals((GstAppSink*) m_implementation->sink, true);
	gst_app_sink_set_drop((GstAppSink*) m_implementation->sink, true);
	gst_app_sink_set_max_buffers((GstAppSink*) m_implementation->sink, 1);
	gst_base_sink_set_sync((GstBaseSink*) m_implementation->sink, false);
	gst_app_sink_set_caps((GstAppSink*) m_implementation->sink, caps);
	gst_caps_unref(caps);
#endif

	const auto prerollCallback = [](auto sink, auto data) { return (GstFlowReturn) preroll(sink, static_cast<GStreamerVideoSource*>(data)); };
	const auto grabFrameCallback = [](auto sink, auto data) { return (GstFlowReturn) grabFrame(sink, static_cast<GStreamerVideoSource*>(data)); };

	GstAppSinkCallbacks callbacks = {nullptr, prerollCallback, grabFrameCallback};

	gst_app_sink_set_callbacks(GST_APP_SINK(m_implementation->sink), &callbacks, this, nullptr);
	gst_element_set_state(GST_ELEMENT(m_implementation->pipeline), GST_STATE_PLAYING);

	m_lastError = B4BError::ok();
}

GStreamerVideoSource::~GStreamerVideoSource() noexcept
{
	gst_element_set_state(GST_ELEMENT(m_implementation->pipeline), GST_STATE_NULL);
	gst_object_unref(m_implementation->sink);
	gst_object_unref(m_implementation->userPipeline);
	gst_object_unref(m_implementation->pipeline);
}

dto::ImageMetadata
GStreamerVideoSource::metadata() const noexcept
{
	return m_frame.metadata();
}

std::error_code
GStreamerVideoSource::nextFrame() noexcept
{
	if (B4BError::ok() != m_lastError)
	{
		return m_lastError;
	}

	const auto predicate = [this]() {
		return frameReady();
	};

	{
		std::unique_lock<decltype(m_mutex)> lock(m_mutex);

		m_frameReadyCondition.wait(lock, predicate);

		if (!m_frameReady)
		{
			// Not exactly an error, but not sure what to return here
			return B4BError::genericError();
		}

		m_frameReady = false;
		m_bufferReady = true;
	}

	m_bufferReadyCondition.notify_all();

	return B4BError::ok();
}

dto::ImageView
GStreamerVideoSource::frame() const noexcept
{
	return m_frame;
}

dto::ImageView
GStreamerVideoSource::frame(std::byte*, std::size_t) const noexcept
{
	// Not implemented
	return dto::ImageView();
}

std::error_code
GStreamerVideoSource::execute(const std::string&) noexcept
{
	return B4BError::ok();
}

int
GStreamerVideoSource::preroll(void*, GStreamerVideoSource* self)
{
	{
		std::unique_lock<decltype(self->m_mutex)> lock(self->m_mutex);
		self->m_bufferReady = true;
	}

	self->m_bufferReadyCondition.notify_all();

	return GST_FLOW_OK;
}

int
GStreamerVideoSource::grabFrame(void* sink, GStreamerVideoSource* self)
{
	const auto predicate = [self]() {
		return self->bufferReady();
	};

	{
		std::unique_lock<decltype(self->m_mutex)> lock(self->m_mutex);
		self->m_bufferReadyCondition.wait(lock, predicate);
	}

	const auto appsink = static_cast<GstAppSink*>(sink);
	const auto sample = gst_app_sink_pull_sample(appsink);

	if (!sample)
	{
		if (gst_app_sink_is_eos(appsink))
		{
			{
				std::unique_lock<decltype(self->m_mutex)> lock(self->m_mutex);
				self->m_endOfStream = true;
			}
			self->m_frameReadyCondition.notify_all();
			return GST_FLOW_OK;
		}
		else
		{
			self->m_lastError = B4BError::genericError();
			return GST_FLOW_ERROR;
		}
	}

	self->m_implementation->sample = std::make_unique<Sample>(sample);
	self->m_frame = self->m_implementation->sample->imageView(self->m_width, self->m_height);
	self->m_frameReady = true;
	self->m_bufferReady = false;
	self->m_frameReadyCondition.notify_all();

	return GST_FLOW_OK;
}
} // namespace neurala
