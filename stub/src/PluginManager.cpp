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

#include <algorithm>
#include <mutex>
#include <string>
#include <unordered_map>

#include "neurala/plugin/PluginArguments.h"
#include "neurala/plugin/PluginErrorCallback.h"
#include "neurala/plugin/PluginManager.h"
#include "neurala/plugin/PluginStatus.h"
#include "neurala/utils/Version.h"

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

		if (className.empty() || !std::all_of(className.cbegin(), className.cend(), [](char c) {
			    return std::isalnum(c) != 0 || c == '_';
		    }))
		{
			// empty name or name with anything except [a-zA-Z0-9\_] is not allowed
			return make_error_code(PluginStatus::invalidName());
		}

		if (m_version.major() != version.major())
		{
			// plugin and host versions mismatch
			return make_error_code(PluginStatus::wrongVersion());
		}

		std::lock_guard<std::mutex> lock{m_classesMutex};
		auto it = m_classes.emplace(className,
		                            PluginRegistrar::ClassMetadata{classConstructor, classDestructor});
		if (!it.second)
		{
			// class already registered
			return make_error_code(PluginStatus::alreadyRegistered());
		}
	}
	catch (...)
	{
		// unknown error - most likely memory allocation error
		return make_error_code(PluginStatus::unknown());
	}

	return make_error_code(PluginStatus::success());
}

} // namespace neurala
