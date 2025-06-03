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

#ifndef NEURALA_VIDEO_DTO_CAMERA_INFO_H
#define NEURALA_VIDEO_DTO_CAMERA_INFO_H

#include <iomanip>
#include <ostream>
#include <string>

namespace neurala
{
namespace dto
{
/**
 * @brief Camera information.
 */
class CameraInfo
{
	std::string m_id;
	std::string m_name;
	std::string m_connection;
	std::string m_type;

public:
	CameraInfo() = default;

	/**
	 * @brief Constructs a new @ref CameraInfo object.
	 *
	 * Most notably, CameraInfo objects represent a means of communication with the Neurala SDK through
	 * the implementation of a CameraDiscoverer.
	 *
	 * @param id unique ID (e.g. MAC address for eBUS cameras)
	 * @param type name used inside initMe() to register the associated VideoSource implementation.
	 * Outside of this context, the type equates to the string representation of the corresponding
	 * ECameraType.
	 * @param name display name of the camera
	 * @param connection plugin-defined connection data (e.g. IP address or directory path)
	 */
	CameraInfo(std::string id, std::string type, std::string name, std::string connection) noexcept
	 : m_id{std::move(id)}, m_name{std::move(name)}, m_connection{std::move(connection)}, m_type{std::move(type)}
	{ }

	/**
	 * @brief Returns if this camera info is for an existing camera.
	 */
	bool valid() const noexcept { return !m_id.empty(); }

	/**
	 * @brief Returns the ID of the camera.
	 *
	 * - For @ref ECameraType::eBUS cameras, this is the MAC address of the camera.
	 * - For @ref ECameraType::usb cameras, this is the id of the camera.  (E.x. the first USB
	 *   connected webcam will have an id of 0.)
	 */
	const std::string& id() const noexcept { return m_id; }

	/**
	 * @brief Returns the name of the camera.
	 */
	const std::string& name() const noexcept { return m_name; }

	/**
	 * @brief Returns the camera type.
	 */
	const std::string& type() const noexcept { return m_type; }

	/**
	 * @brief Returns how the camera is connected.
	 *
	 * - For @ref ECameraType::eBUS cameras, this is the IP address of the camera.
	 * - For @ref ECameraType::simulated cameras, this is the directory of images that will be used
	 *   to simulate a camera.
	 * - For @ref ECameraType::usb cameras, this will be @c "usb".
	 */
	const std::string& connection() const noexcept { return m_connection; }

	/**
	 * @brief Comparison operator.  Compares all fields except the camera's name.
	 */
	friend bool operator==(const CameraInfo& x, const CameraInfo& y) noexcept
	{
		return x.id() == y.id();
	}

	friend bool operator!=(const CameraInfo& x, const CameraInfo& y) noexcept { return !(x == y); }

	friend std::ostream& operator<<(std::ostream& os, const CameraInfo& cameraInfo)
	{
		return os << "{id = " << std::quoted(cameraInfo.id())
		          << ", type = " << std::quoted(cameraInfo.type())
		          << ", name = " << std::quoted(cameraInfo.name())
		          << ", connection = " << std::quoted(cameraInfo.connection()) << '}';
	}
};

} // namespace dto
} // namespace neurala

#endif // NEURALA_VIDEO_CAMERA_INFO_H
