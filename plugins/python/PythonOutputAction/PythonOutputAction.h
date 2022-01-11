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

#ifndef NEURALA_PYTHON_OUTPUT_ACTION_H
#define NEURALA_PYTHON_OUTPUT_ACTION_H

#include <neurala/plugin/PluginArguments.h>
#include <neurala/plugin/PluginBindings.h>
#include <neurala/plugin/PluginErrorCallback.h>
#include <neurala/plugin/PluginManager.h>
#include <neurala/plugin/PluginStatus.h>
#include <neurala/utils/ResultsOutput.h>

#include "PythonPlugin.h"


namespace neurala {
	namespace PythonPlugin {
		class PythonOutputAction final : public neurala::ResultsOutput
		{

		public:
			PyObject* m_outputcallable = nullptr;

			PythonOutputAction(PyObject* callable) : m_outputcallable{callable} {};

			static void* create(neurala::PluginArguments& args, neurala::PluginErrorCallback& error) {
				constexpr char* kDefaultCallableName = "outputFrame";

				ACQUIRE_GIL;

				const auto& spec = args.get<0, const std::string>();
				PyObject* obj = objFromSpec(spec, kDefaultCallableName);

				RELEASE_GIL;
				// PythonOutputAction steals a reference to the callable object, it will be dropped when destroy() is called
				return new PythonOutputAction(obj);
			}

			static void destroy(void* p) {
				auto *pobj = reinterpret_cast<PythonOutputAction*>(p);
				Py_XDECREF(pobj->m_outputcallable);
				delete pobj;
			}

			void operator()(const std::string& metadata, const dto::ImageView* view) noexcept final;
		};

	} // namespace PythonPlugin
} // namespace neurala

#endif // NEURALA_PYTHON_OUTPUT_ACTION_H