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

#ifndef NEURALA_PLUGIN_NEURALA_PLUGIN_STATUS_H
#define NEURALA_PLUGIN_NEURALA_PLUGIN_STATUS_H

#include <system_error>

#include "neurala/exports.h"
#include "neurala/plugin/PluginBindings.h"

/// @brief Definition of the plugin status error domain
NEURALA_PUBLIC const std::error_category& neuralaPluginStatusCategory() noexcept;

/**
 * @brief Creates a std::error_condition from a @p NeuralaPluginStatus
 * @param status The code to convert
 */
NEURALA_PUBLIC std::error_condition
make_error_condition(NeuralaPluginStatus status) noexcept; // NOLINT

/**
 * @brief Creates a std::error_condition from a @p NeuralaPluginStatus
 *
 * @note While @p NeuralaPluginStatus can be used as a std::error_code, you are encouraged to create
 *       your own error codes and define equivalence relationship with this status.
 *
 * @param status The code to convert
 */
NEURALA_PUBLIC std::error_code make_error_code(NeuralaPluginStatus status) noexcept; // NOLINT

namespace std
{
template<>
struct is_error_condition_enum<NeuralaPluginStatus> : true_type
{ };
} // namespace std

#endif // NEURALA_PLUGIN_NEURALA_PLUGIN_STATUS_H