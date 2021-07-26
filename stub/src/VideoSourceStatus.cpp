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

#include "neurala/video/VideoSourceStatus.h"

#include "neurala/error/B4BError.h"

namespace neurala
{
namespace
{
class VideoSourceStatusCat final : public std::error_category
{
public:
	const char* name() const noexcept override { return "VideoSourceStatus"; }

	std::string message(int c) const override
	{
		return enumToString(static_cast<VideoSourceStatus>(c));
	}

	bool equivalent(int code, const std::error_condition& condition) const noexcept override
	{
		// Avoid infinite loops
		if (*this == condition.category())
		{
			return code == condition.value();
		}

		switch (static_cast<VideoSourceStatus>(code))
		{
			case VideoSourceStatus::success:
				return B4BError::ok == condition;
			case VideoSourceStatus::error:
				return B4BError::genericError == condition;
			case VideoSourceStatus::unknown:
				return B4BError::unknown == condition;
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

		switch (static_cast<VideoSourceStatus>(condition))
		{
			case VideoSourceStatus::success:
				return code == B4BError::ok;
			case VideoSourceStatus::error:
				return code == B4BError::genericError;
			case VideoSourceStatus::unknown:
				return code == B4BError::unknown;
			default:
				return false;
		}
	}
};
} // namespace

const std::error_category&
videoSourceStatusCategory() noexcept
{
	static const VideoSourceStatusCat cat;
	return cat;
}


std::error_condition
make_error_condition(VideoSourceStatus res) noexcept
{
	return std::error_condition(static_cast<int>(res), videoSourceStatusCategory());
}


std::error_code
make_error_code(VideoSourceStatus res) noexcept
{
	return std::error_code(static_cast<int>(res), videoSourceStatusCategory());
}

} // namespace neurala