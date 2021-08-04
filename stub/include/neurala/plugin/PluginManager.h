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

#ifndef NEURALA_PLUGIN_PLUGIN_MANAGER_H
#define NEURALA_PLUGIN_PLUGIN_MANAGER_H

#include "neurala/plugin/PluginArguments.h"
#include "neurala/plugin/PluginBindings.h"
#include "neurala/plugin/PluginErrorCallback.h"
#include "neurala/utils/Version.h"

extern "C"
{
	struct NEURALA_PUBLIC NeuralaPluginManager
	{
		// Ignored member to ensure the struct has a non-zero size which
		// causes a warning when compiling the header against both C and C++.
		// see: -Wextern-c-compat
		std::size_t m_unused{0};

		NeuralaPluginManager() = default;
		NeuralaPluginManager(const NeuralaPluginManager&) = delete;
		NeuralaPluginManager(NeuralaPluginManager&&) = delete;
		NeuralaPluginManager& operator=(const NeuralaPluginManager&) = delete;
		NeuralaPluginManager& operator=(NeuralaPluginManager&&) = delete;
		virtual ~NeuralaPluginManager() = default;
	};
}

namespace neurala
{
/**
 * @brief Class used to register user defined types while initializing a plugin.
 *
 * The pointer to NeuralaPluginManager passed through the call to initMe should first be
 * dynamically cast as a PluginRegistrar to gain access to the registerPlugin function.
 */
class NEURALA_PUBLIC PluginRegistrar : public NeuralaPluginManager
{
public:
	using ClassConstructorPtr = void* (*)(PluginArguments&, PluginErrorCallback&);
	using ClassDestructorPtr = void (*)(void*);

	struct ClassMetadata
	{
		ClassConstructorPtr create{};
		ClassDestructorPtr destroy{};
	};

	/**
	 * @brief Registers a plugin with the plugin manager.
	 *
	 * @note This function is called by a plugin during its initialization.
	 *
	 * @param name    plugin name
	 * @param version plugin version
	 *
	 * @return status of registration
	 */
	template<class Plugin>
	NeuralaPluginStatus registerPlugin(const char* name, const Version& version)
	{
		return registerClass(name, version, Plugin::create, Plugin::destroy);
	}

private:
	/**
	 * @brief Registers a class with the plugin manager.
	 *
	 * @note This function is called by a plugin during its initialization.
	 *
	 * @param name class name
	 * @param version class version
	 * @param classConstructor class instance construction function
	 * @param classDestructor  class instance destruction function
	 *
	 * @return status of registration
	 */
	virtual NeuralaPluginStatus registerClass(const char* name,
	                                          const Version& version,
	                                          ClassConstructorPtr classConstructor,
	                                          ClassDestructorPtr classDestructor) = 0;
};

} // namespace neurala

#endif // NEURALA_PLUGIN_PLUGIN_MANAGER_H
