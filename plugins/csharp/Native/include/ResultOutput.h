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

#ifndef NEURALA_CSHARP_RESULTS_OUTPUT_H
#define NEURALA_CSHARP_RESULTS_OUTPUT_H

#include "neurala/plugin/PluginArguments.h"
#include "neurala/plugin/PluginBindings.h"
#include "neurala/plugin/PluginErrorCallback.h"
#include "neurala/plugin/PluginManager.h"
#include "neurala/plugin/PluginStatus.h"
#include "neurala/utils/Version.h"
#include "neurala/utils/ResultsOutput.h"

namespace neurala
{
class PLUGIN_API CSharpResultOutput : public ResultsOutput
{
public:
	virtual void operator()(const std::string& metadata, const dto::ImageView* image) noexcept override;

	static void* create(PluginArguments&, PluginErrorCallback&);
	static void destroy(void* p);
};
} // namespace neurala

#endif // NEURALA_CSHARP_RESULTS_OUTPUT_H
