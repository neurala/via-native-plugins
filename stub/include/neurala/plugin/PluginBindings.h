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

#ifndef NEURALA_PLUGIN_PLUGIN_BINDINGS_H
#define NEURALA_PLUGIN_PLUGIN_BINDINGS_H

#include "neurala/config/os.h"

#ifdef NEURALA_OS_WINDOWS
#ifndef PLUGIN_API
#ifdef NEURALA_EXPORT_PLUGIN // building the library
#define PLUGIN_API __declspec(dllexport)
#else // using the library
#define PLUGIN_API __declspec(dllimport)
#endif // NEURALA_EXPORT_PLUGIN
#endif // PLUGIN_API
#else
#define PLUGIN_API __attribute__((visibility("default")))
#endif // NEURALA_OS_WINDOWS

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 * @brief Base class for @ref neurala::PluginManager.
	 */
	typedef struct NeuralaPluginManager NeuralaPluginManager;

	/**
	 * @brief Plugin status codes.
	 */
	typedef enum
	{
		success,
		unknown,
		wrongVersion,
		invalidName,
		alreadyRegistered
	} NeuralaPluginStatus;

	/// @brief Plugin cleanup function pointer, returned by the initialisation if successful.
	typedef int (*NeuralaPluginExitFunction)();

	/**
	 * @brief Plugin initialization function.
	 *
	 * This is the library entry point.
	 *
	 * @param pluginManager @ref neurala::PluginManager instance responsible for the plugin
	 * @param status        initialization status
	 *
	 * @return exit function of the plugin or @c nullptr if initialization failed.
	 */
	typedef NeuralaPluginExitFunction (*NeuralaPluginEntryFunction)(NeuralaPluginManager* pluginManager,
	                                                                NeuralaPluginStatus* status);

	/// @brief Plugin entry point, needs to be implemented in plugin implementation.
	PLUGIN_API NeuralaPluginExitFunction initMe(NeuralaPluginManager*, NeuralaPluginStatus*);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NEURALA_PLUGIN_PLUGIN_BINDINGS_H
