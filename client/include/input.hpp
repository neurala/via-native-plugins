#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <system_error>
#include <vector>

#include <neurala/plugin/detail/PluginBindings.h>
#include <neurala/plugin/detail/PluginManager.h>
#include <neurala/video/VideoSource.h>

#include "client.hpp"

namespace neurala
{
class Input final : public VideoSource
{
public:
	static void* create(PluginArguments&, PluginErrorCallback&) { return new Input; }
	static void destroy(void* p) { delete reinterpret_cast<Input*>(p); }

	[[nodiscard]] ImageMetadata metadata() const final { return cachedMetadata(); }

	[[nodiscard]] NextFrameResult nextFrame() final;

	[[nodiscard]] ImageView frame() final { return {cachedMetadata(), m_frames.back().data()}; }

	[[nodiscard]] ImageView frame(std::byte* data, std::size_t size) final;

	[[nodiscard]] std::error_code execute(const std::string& action) final { return {}; }

private:
	const ImageMetadata& cachedMetadata() const;

	mutable std::optional<ImageMetadata> m_metadata;
	std::vector<std::vector<std::byte>> m_frames;
};

} // namespace neurala
