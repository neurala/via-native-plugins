/*
 * This file is part of Neurala SDK.
 * Copyright Neurala Inc. 2013-2022. All rights reserved.
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

#include <algorithm>

#include "neurala/plugin/PluginStatus.h"

#include "PythonOutputAction.h"

#define REGISTER_CLASS(cls, name, version) \
	*status = pm.registerPlugin<cls>(name, version); \
	if (*status != neurala::PluginStatus::success() && \
	    *status != neurala::PluginStatus::alreadyRegistered() ) \
		{ return nullptr; }

namespace
{
int
exitHere()
{
	return 0;
}

} // namespace

extern "C" NeuralaPluginExitFunction
initMe(NeuralaPluginManager* pluginManager, std::error_code* status)
{
	auto kPluginVersion = neurala::Version(1, 0);
	auto& pm = *dynamic_cast<neurala::PluginRegistrar*>(pluginManager);

	neurala::initializePython(L"PythonOutputAction");

	REGISTER_CLASS(neurala::PythonPlugin::PythonOutputAction, "PythonOutputAction", kPluginVersion);

	return exitHere;

}

namespace neurala {
	namespace PythonPlugin {

		void PythonOutputAction::operator()(const std::string& metadata, const dto::ImageView* view) noexcept
		{
			PyObject *args, *result;

			ACQUIRE_GIL;

			if (m_outputcallable == nullptr)
			{
				DEBUG("no output callable\n");
				goto cleanup;
			}

			args = PyTuple_Pack(1, PyUnicode_FromString(metadata.c_str()));

			// a more sophisticated plugin could make use of the SWIG layer here to also pass the ImageView
			result = PyObject_Call(m_outputcallable, args, NULL);
			if (result == NULL) {
				DEBUG("error occurred in call\n");
				// Python exception information is available here using e.g. PyErr_Print() if desired
				goto cleanup;
			}
			cleanup:
				Py_XDECREF(args);
				Py_XDECREF(result);
				RELEASE_GIL;
		}

	} // namespace PythonPlugin
} // namespace neurala

