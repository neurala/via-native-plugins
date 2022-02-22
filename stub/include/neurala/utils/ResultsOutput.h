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

#ifndef NEURALA_UTILS_RESULTS_OUTPUT_H
#define NEURALA_UTILS_RESULTS_OUTPUT_H

#include <functional>
#include <string>
#include <string_view>

#include "neurala/exports.h"
#include "neurala/image/views/dto/ImageView.h"
#include "neurala/utils/Options.h"

namespace neurala
{
/**
 * @brief A type representing the status of a pipeline job upon stopping.
 */
struct ResultsOutputStatus final
{
	///< Indicates that the pipeline job has terminated normally.
	static constexpr ResultsOutputStatus stopped() { return {0}; }
	///< Indicates that the pipeline job has terminated abnormally.
	static constexpr ResultsOutputStatus faulted() { return {1}; }

	constexpr operator int() const noexcept { return m_value; }

	int m_value;
};

/**
 * @brief Base type for output actions implemented as plugins.
 */
class ResultsOutputBase
{
public:
	/// @brief Virtual destructor.
	virtual ~ResultsOutputBase() = default;

	/**
	 * @brief Called at the start of a pipeline job.
	 *
	 * @param id The ID of the pipeline job that is starting. This value can be
	 *           used as a discriminator to track which job is starting.
	 */
	virtual void onStart(std::string_view id) noexcept { }

	/**
	 * @brief Called at the end of a pipeline job.
	 *
	 * @param id The ID of the pipeline job that is starting. This value can be
	 *           used as a discriminator to track which job is stopping.
	 * @param status The reason for stopping.
	 */
	virtual void onStop(std::string_view id, ResultsOutputStatus status) noexcept { }
};

/**
 * @brief Base type for public plugin output actions. Plugin implementers
 *        should inherit from this type.
 */
class ResultsOutput : public ResultsOutputBase
{
public:
	/**
	 * @brief Function call operator for invoking the output action.
	 *
	 * In relation to the VideoSource implementation, calls may be expected following requests for
	 * frame data and preceding those that query the status of upcoming frames (nextFrame).
	 *
	 * @param metadata A JSON document containing information about the result.
	 * @param image A pointer to an image view, which may be null if no frame
	 *              is available or could be retrieved.
	 */
	virtual void operator()(const std::string& metadata, const dto::ImageView* image) noexcept = 0;
};

} // namespace neurala

#endif // NEURALA_UTILS_RESULTS_OUTPUT_H
