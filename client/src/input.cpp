#include "input.h"

namespace neurala
{
NextFrameResult
Input::nextFrame()
{
	try
	{
		std::vector<std::byte> frameBuffer(cachedMetadata().sizeBytes());
		Client::get().frame(frameBuffer.data());
		m_frames.emplace_back(std::move(frameBuffer));
		return NextFrameResult::Status::success;
	}
	catch (...)
	{
		std::cerr << "Failed retrieving frame from the server.\n";
		return NextFrameResult::Status::error;
	}
}

ImageView
Input::frame(std::byte* data, std::size_t size)
{
	const ImageMetadata& md = cachedMetadata();
	if (size < md.sizeBytes())
	{
		std::cerr << "Insufficient capacity in B4B buffer.\n";
	}
	else
	{
		std::copy(cbegin(m_frames.back()), cend(m_frames.back()), data);
		m_frames.pop_back();
	}
	return {md, data};
}

const ImageMetadata&
Input::cachedMetadata() const
{
	if (!m_metadata)
	{
		m_metadata = Client::get().metadata();
	}
	return *m_metadata;
}

} // namespace neurala
