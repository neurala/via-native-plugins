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

#ifndef NEURALA_PYTHON_PLUGIN_H
#define NEURALA_PYTHON_PLUGIN_H

#include <neurala/plugin/PluginArguments.h>
#include <neurala/plugin/PluginBindings.h>
#include <neurala/plugin/PluginErrorCallback.h>
#include <neurala/plugin/PluginManager.h>
#include <neurala/plugin/PluginStatus.h>
#include <neurala/utils/ResultsOutput.h>


#include "Python.h"

// These macros acquire/release the Global Interpreter Lock and can be called any number of times on a single thread
// as long as they are used in pairs.
#define ACQUIRE_GIL \
			PyGILState_STATE gil_;  \
			gil_ = PyGILState_Ensure();

#define RELEASE_GIL \
			PyGILState_Release(gil_);


namespace neurala {
	namespace PythonPlugin {
		class PythonOutputAction final : public neurala::ResultsOutput
		{

		public:
			PyObject* m_outputcallable = nullptr;

			static void* create(neurala::PluginArguments& args, neurala::PluginErrorCallback& error) {
				PythonOutputAction *ptr = nullptr;
				PyObject *mod, *func, *spec, *callableName, *moduleName, *result;

				ACQUIRE_GIL;

				const auto& callableSpec = args.get<0, const std::string>();
				spec = PyUnicode_FromString(callableSpec.c_str());

				PyObject* argstup = PyTuple_Pack(1, PyUnicode_FromString(":"));
				PyObject* restup = PyObject_Call(PyObject_GetAttrString(spec, "partition"), argstup, NULL);
				Py_XDECREF(argstup);
				if (restup == NULL)
				{
					error("Erroneous callableSpec");
					goto cleanup;
				}

				moduleName = PyTuple_GET_ITEM(restup, 0);  // borrowed reference
				callableName = PyTuple_GET_ITEM(restup, 2);  // borrowed reference

				std::cout << "moduleName=" << PyUnicode_AS_DATA(moduleName) << ", callableName=" << PyUnicode_AS_DATA(callableName) << "\n";

				mod = PyImport_Import(moduleName);
				if (mod == NULL) {
					error("can't import module");
					goto cleanup;
				}
				func = PyObject_GetAttr(mod, callableName);
				if (func == NULL) {
					error("can't retrieve function");
					Py_XDECREF(mod);
					goto cleanup;
				}

				ptr = new PythonOutputAction;
				// PythonOutputAction steals the above reference to this object, it will be dropped when destroy() is called
				ptr->m_outputcallable = func;

			cleanup:
				Py_XDECREF(mod);
				RELEASE_GIL;

				return ptr;
			}

			static void destroy(void* p) {
				Py_Finalize();
				delete reinterpret_cast<PythonOutputAction*>(p);
			}

			void operator()(const std::string& metadata, const dto::ImageView* view) noexcept final;
		};

	} // namespace PythonPlugin
} // namespace neurala

#endif // NEURALA_PYTHON_PLUGIN_H
