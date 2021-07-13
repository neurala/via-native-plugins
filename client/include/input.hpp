#pragma once

#include <cstddef>
#include <optional>
#include <string>
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

	[[nodiscard]] NextFrameResult nextFrame() final { return NextFrameResult::Status::success; }

	[[nodiscard]] ImageView frame() final
	{
		const ImageMetadata& md = cachedMetadata();
		std::cout << "SIZE IN BYTES: " << md.sizeBytes() << '\n';
		Client::get().frame(m_frames.emplace_back(md.sizeBytes()).data());
		std::cout << "FRAME SIZE: " << m_frames.back().size() << std::endl;
		std::for_each(rbegin(m_frames.back()), rbegin(m_frames.back()) + 50, [](std::byte const b) {
			std::cout << (int)b << std::endl;
		});
		return {md, m_frames.back().data()};
	}

	[[nodiscard]] ImageView frame(std::byte* data, std::size_t size) final
	{
		const ImageMetadata& md = cachedMetadata();
		if (size < md.sizeBytes())
			std::cerr << "Insufficient capacity in B4B buffer.\n";
		else
			Client::get().frame(data);
		return {md, data};
	}

	[[nodiscard]] std::error_code execute(const std::string& action) final { return {}; }

private:
	const ImageMetadata& cachedMetadata() const
	{
		if (!m_metadata)
			m_metadata = Client::get().metadata();
		return *m_metadata;
	}

	mutable std::optional<ImageMetadata> m_metadata;
	std::vector<std::vector<std::byte>> m_frames;
};

} // namespace neurala
