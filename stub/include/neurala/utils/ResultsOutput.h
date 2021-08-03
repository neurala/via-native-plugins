/*
 * This file is part of Neurala SDK.
 *  Copyright Neurala Inc. 2013-2021. All rights reserved.
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

#ifndef NEURALA_UTILS_RESULTS_OUTPUT_H
#define NEURALA_UTILS_RESULTS_OUTPUT_H

#include <functional>
#include <string>
#include <string_view>

#include "neurala/exports.h"
#include "neurala/image/views/ImageView.h"
#include "neurala/utils/Option.h"

namespace neurala
{
/**
 * @brief An enumeration type representing the status of a pipeline job upon
 *        stopping.
 */
enum class EResultsOutputStatus
{
	stopped, ///< Indicates that the pipeline job has terminated normally.
	faulted  ///< Indicates that the pipeline job has terminated abnormally.
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
	virtual void onStop(std::string_view id, EResultsOutputStatus status) noexcept { }
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
	 * @param metadata A JSON document containing information about the result.
	 * @param image A pointer to an image view, which may be null if no frame
	 *              is available or could be retrieved.
	 */
	virtual void operator()(const std::string& metadata, const ImageView* image) noexcept = 0;
};

} // namespace neurala

#endif // NEURALA_UTILS_RESULTS_OUTPUT_H
