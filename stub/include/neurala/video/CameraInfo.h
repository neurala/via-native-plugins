/*
 * This file is part of Neurala SDK.
 * Copyright Neurala Inc. 2013-2021. All rights reserved.
 *
 * Except as expressly permitted in the accompanying License Agreement, if at all, (a) you shall
 * not license, sell, rent, lease, transfer, assign, distribute, display, host, outsource, disclose
 * or otherwise commercially exploit or make this source code available to any third party; (b) you
 * shall not modify, make derivative works of, disassemble, reverse compile or reverse engineer any
 * part of the SDK; (c) You shall not access the SDK in order to build a similar or competitive
 * product or service; (d) no part of the this source may be copied, reproduced, distributed,
 * republished, downloaded, displayed, posted or transmitted in any form or by any means, including
 * but not limited to electronic, mechanical, photocopying, recording or other means; and (e) any
 * future release, update, or other addition to functionality of the SDK shall be subject to the
 * terms of the accompanying License Agreement. You must reproduce, on all copies made by you or
 * for you, and must not remove, alter, or obscure in any way all proprietary rights notices
 * (including copyright notices) of Neurala Inc or its suppliers on or within the copies of the
 * SDK. Any sample code provided with the SDK and designated as such are for illustrative purposes
 * only and are not to be included in your applications.
 *
 * In cases when the accompanying License Agreement permits redistribution of this file, the above
 * notice shall be reproduced its entirety in every copy of a distributed version of this file.
 */

#ifndef NEURALA_VIDEO_CAMERA_INFO_H
#define NEURALA_VIDEO_CAMERA_INFO_H

#include <iomanip>
#include <ostream>
#include <string>

#include "neurala/meta/enum.h"

namespace neurala
{
/**
 * @brief Camera type.
 */
enum class ECameraType
{
	unknown,
	/// simulated camera
	simulated,
	/// USB camera
	usb,
	/// GEV accessible camera
	gev,
	/// Pleora SDK accessible camera
	eBUS
};

template<>
class MetaEnum<ECameraType> : public MetaEnumRegister<ECameraType>
{
public:
	static constexpr const auto values = enumRegisterValues(
	 NEURALA_META_ENUM_ENTRY(ECameraType, unknown),
	 NEURALA_META_ENUM_ENTRY(ECameraType, simulated),
	 NEURALA_META_ENUM_ENTRY(ECameraType, usb),
	 NEURALA_META_ENUM_ENTRY(ECameraType, gev),
	 NEURALA_META_ENUM_ENTRY(ECameraType, eBUS));

	static constexpr const auto fallbackValue = values[0];
};

/**
 * @brief Camera information.
 */
class CameraInfo
{
	std::string m_id;
	std::string m_name;
	std::string m_connection;
	ECameraType m_type{ECameraType::unknown};

public:
	CameraInfo() = default;

	/**
	 * @brief Constructs a new @ref CameraInfo object.
	 */
	CameraInfo(std::string id, ECameraType type, std::string name, std::string connection) noexcept
	 : m_id{std::move(id)}, m_name{std::move(name)}, m_connection{std::move(connection)}, m_type{type}
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
	ECameraType type() const noexcept { return m_type; }

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
		return x.id() == y.id() && x.type() == y.type() && x.connection() == y.connection();
	}

	friend bool operator!=(const CameraInfo& x, const CameraInfo& y) noexcept { return !(x == y); }

	friend std::ostream& operator<<(std::ostream& os, const CameraInfo& cameraInfo)
	{
		return os << "{id = " << std::quoted(cameraInfo.id()) << ", type = " << cameraInfo.type()
		          << ", name = " << std::quoted(cameraInfo.name())
		          << ", connection = " << std::quoted(cameraInfo.connection()) << '}';
	}
};

} // namespace neurala

#endif // NEURALA_VIDEO_CAMERA_INFO_H