/*
 * Copyright (c) Neurala Inc. 2013-2021
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
	std::error_code registerPlugin(const char* name, const Version& version)
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
	virtual std::error_code registerClass(const char* name,
	                                      const Version& version,
	                                      ClassConstructorPtr classConstructor,
	                                      ClassDestructorPtr classDestructor) = 0;
};

} // namespace neurala

#endif // NEURALA_PLUGIN_PLUGIN_MANAGER_H
