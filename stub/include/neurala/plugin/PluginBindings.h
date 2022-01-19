/*
 * Copyright Neurala Inc. 2013-2022
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

#ifndef NEURALA_PLUGIN_PLUGIN_BINDINGS_H
#define NEURALA_PLUGIN_PLUGIN_BINDINGS_H

#include <system_error>

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
	                                                                std::error_code* status);

	/// @brief Plugin entry point, needs to be implemented in plugin implementation.
	PLUGIN_API NeuralaPluginExitFunction initMe(NeuralaPluginManager*, std::error_code*);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // NEURALA_PLUGIN_PLUGIN_BINDINGS_H
