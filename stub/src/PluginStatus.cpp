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

#include "neurala/error/B4BError.h"

#include "neurala/plugin/PluginStatus.h"

namespace
{
/**
 * @brief Returns a string representation of @p status.
 */
const char*
pluginStatusToStr(neurala::PluginStatus status)
{
	switch (status)
	{
		case neurala::PluginStatus::success:
			return "success";
		case neurala::PluginStatus::wrongVersion:
			return "incompatible version";
		case neurala::PluginStatus::invalidName:
			return "invalid name";
		case neurala::PluginStatus::alreadyRegistered:
			return "already registered";
		default:
			return "unknown";
	}
}

class neurala::PluginStatusCategory : public std::error_category
{
public:
	const char* name() const noexcept override { return "neurala::PluginStatus"; }

	std::string message(int c) const override
	{
		return pluginStatusToStr(static_cast<neurala::PluginStatus>(c));
	}

	bool equivalent(int code, const std::error_condition& condition) const noexcept override
	{
		// Avoid infinite loops
		if (*this == condition.category())
		{
			return code == condition.value();
		}

		switch (static_cast<neurala::PluginStatus>(code))
		{
			case neurala::PluginStatus::success:
				return neurala::B4BError::ok == condition;

			case neurala::PluginStatus::wrongVersion:
			case neurala::PluginStatus::invalidName:
				return neurala::B4BError::genericError == condition;
			case neurala::PluginStatus::unknown:
				return neurala::B4BError::unknown == condition;
			default:
				return false;
		}
	}

	bool equivalent(const std::error_code& code, int condition) const noexcept override
	{
		// Avoid infinite loops
		if (code.category() == *this)
		{
			return code.value() == condition;
		}

		switch (static_cast<neurala::PluginStatus>(condition))
		{
			case neurala::PluginStatus::success:
				return code == neurala::B4BError::ok;
			case neurala::PluginStatus::wrongVersion:
			case neurala::PluginStatus::invalidName:
				return code == neurala::B4BError::genericError;
			case neurala::PluginStatus::unknown:
				return code == neurala::B4BError::unknown;
			default:
				return false;
		}
	}
};
} // namespace

namespace neurala
{
const std::error_category&
pluginStatusCategory() noexcept
{
	static const PluginStatusCategory category;
	return category;
}

std::error_condition
make_error_condition(PluginStatus status) noexcept // NOLINT
{
	return std::error_condition{static_cast<int>(status), pluginStatusCategory()};
}

std::error_code
make_error_code(PluginStatus status) noexcept // NOLINT
{
	return std::error_code{static_cast<int>(status), pluginStatusCategory()};
}
} // namespace neurala
