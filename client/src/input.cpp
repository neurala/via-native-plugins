#include "input.hpp"

namespace neurala
{
ImageView
Input::frame()
{
	const ImageMetadata& md = cachedMetadata();
	Client::get().frame(m_frames.emplace_back(md.sizeBytes()).data());
	return {md, m_frames.back().data()};
}

ImageView
Input::frame(std::byte* data, std::size_t size)
{
	const ImageMetadata& md = cachedMetadata();
	if (size < md.sizeBytes())
		std::cerr << "Insufficient capacity in B4B buffer.\n";
	else
		Client::get().frame(data);
	return {md, data};
}

const ImageMetadata&
Input::cachedMetadata() const
{
	if (!m_metadata)
		m_metadata = Client::get().metadata();
	return *m_metadata;
}

} // namespace neurala
