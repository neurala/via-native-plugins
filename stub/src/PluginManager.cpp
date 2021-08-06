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

#include <string>
#include <algorithm>
#include <mutex>
#include <unordered_map>

#include "neurala/utils/Version.h"
#include "neurala/plugin/PluginStatus.h"
#include "neurala/plugin/PluginManager.h"
#include "neurala/plugin/PluginArguments.h"
#include "neurala/plugin/PluginErrorCallback.h"

namespace neurala
{

// Classes registered by plugins and available for instantiation
std::unordered_map<std::string, PluginRegistrar::ClassMetadata> m_classes;

// Protects class registry (m_classes)
std::mutex m_classesMutex;

// Manager version.
constexpr Version m_version{1, 0};

std::error_code
registerClass(const char* name,
              const Version& version,
              PluginRegistrar::ClassConstructorPtr classConstructor,
              PluginRegistrar::ClassDestructorPtr classDestructor)
{
	try
	{
		std::string className = name;

		if (className.empty()
		    || !std::all_of(className.cbegin(), className.cend(),
		                    [](char c) { return std::isalnum(c) != 0 || c == '_'; }))
		{
			// empty name or name with anything except [a-zA-Z0-9\_] is not allowed
			return make_error_code(PluginStatus::invalidName);
		}

		if (m_version.major() != version.major())
		{
			// plugin and host versions mismatch
			return make_error_code(PluginStatus::wrongVersion);
		}

		std::lock_guard<std::mutex> lock{m_classesMutex};
		auto it = m_classes.emplace(className,
		                            PluginRegistrar::ClassMetadata{classConstructor, classDestructor});
		if (!it.second)
		{
			// class already registered
			return make_error_code(PluginStatus::alreadyRegistered);
		}
	}
	catch (...)
	{
		// unknown error - most likely memory allocation error
		return make_error_code(PluginStatus::unknown);
	}

	return make_error_code(PluginStatus::success);
}

} // namespace neurala
