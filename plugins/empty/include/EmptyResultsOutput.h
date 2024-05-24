/*
 * Copyright Neurala Inc. 2013-2024
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

#ifndef NEURALA_EMPTY_RESULTS_OUTPUT_H
#define NEURALA_EMPTY_RESULTS_OUTPUT_H

#include <neurala/plugin/PluginArguments.h>
#include <neurala/plugin/PluginErrorCallback.h>
#include <neurala/utils/ResultsOutput.h>

namespace neurala::plug::empty
{
class ResultsOutput final : public neurala::ResultsOutput
{
public:
	static void* create(PluginArguments&, PluginErrorCallback&) { return new ResultsOutput; }
	static void destroy(void* p) { delete reinterpret_cast<ResultsOutput*>(p); }

	/**
	 * @brief Function call operator for invoking the output action.
	 *
	 * @param metadata A JSON document containing information about the result.
	 * @param image A pointer to an image view, which may be null if no frame
	 *              is available or could be retrieved.
	 */
	void operator()(const std::string&, const dto::ImageView*) noexcept final { }
};

} // namespace neurala::plug::empty

#endif // NEURALA_EMPTY_RESULTS_OUTPUT_H
