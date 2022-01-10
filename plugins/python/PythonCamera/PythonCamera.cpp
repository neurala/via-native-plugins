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
#include <iostream>
#include <thread>

#include "PythonCamera.h"

#include "neurala/plugin/PluginStatus.h"

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

	neurala::initializePython(L"PythonCamera");

	REGISTER_CLASS(neurala::PythonCamera::Discoverer, "libPythonCameraDiscoverer", kPluginVersion);
	REGISTER_CLASS(neurala::PythonCamera::Source, "PythonCamera", kPluginVersion);

	return exitHere;
}

namespace neurala {
	namespace PythonCamera {

		std::vector<neurala::dto::CameraInfo>
		Discoverer::operator()() const noexcept
		{
			return {{"pycamera1",
					"PythonCamera",
					"Python Camera #1",
					"pycamera1 parameters"}};
		}

		void*
		Discoverer::create(neurala::PluginArguments&, neurala::PluginErrorCallback& error)
		{
			Discoverer* p = nullptr;

			try
			{
				p = new Discoverer();
			}
			catch (const std::exception& e)
			{
				error(e.what());
			}

			return p;
		}

		void
		Discoverer::destroy(void* p)
		{
			delete static_cast<Discoverer*>(p);
		}


		/*
		0 on success
		-1 on error
		*/
		int
		pyImg(Py_buffer* buf)
		{
			DEBUG("acquiring the GIL\n");
			PyGILState_STATE gil = PyGILState_Ensure();

			// get module dict for globals/locals
			PyObject *m, *d, *v;
			m = PyImport_AddModule("__main__");
			if (m == NULL)
			{
				return -1;
			}
			d = PyModule_GetDict(m);

			// NOTE:20220110:jgerity:here we evaluate a fixed program to create a 300x300 image
			// See PythonOutputAction for an example of executing Python source from a file
			const char* prog = "b'\\xFF\\x00\\x00'*300*100 + b'\\x00\\xFF\\x00'*300*100 + b'\\x00\\x00\\xFF'*300*100";
			v = PyRun_String(prog, Py_eval_input, d, d);
			if (v == NULL)
			{
				return -1;
			}

			int err = bufferFromPyObject(v, buf);
			if (err != 0)
			{
 				DEBUG("Failed to get buffer from python object\n");
				return -1;
			}

			Py_XDECREF(v);

			DEBUG("releasing the GIL\n");
			PyGILState_Release(gil);
			return 0;
		}

		Source::Source(const neurala::dto::CameraInfo& cameraInfo, const neurala::Options& options)
		{
			const neurala::dto::ImageMetadata md{metadata()};
			const auto frameBufferSize{md.width() * md.height() * 3};
			m_frame = std::make_unique<std::uint8_t[]>(frameBufferSize);

			Py_buffer buf;
			pyImg(&buf);
			const std::uint8_t* pdata = reinterpret_cast<const std::uint8_t*>(buf.buf);
			std::copy_n(pdata, md.width() * md.height() * 3, &(m_frame[0]));
		}

		neurala::dto::ImageView
		Source::frame(std::byte* data, std::size_t size) const noexcept
		{
			neurala::dto::ImageMetadata md{metadata()};

			std::copy_n(reinterpret_cast<const std::byte*>(m_frame.get()), md.width() * md.height() * 3, data);
			return {std::move(metadata()), data};
		}

		std::error_code
		Source::execute(const std::string& action) noexcept
		{
			return std::error_code{};
		}

		void*
		Source::create(neurala::PluginArguments& arguments, neurala::PluginErrorCallback& error)
		{
			VideoSource* p = nullptr;

			try
			{
				const auto& cameraInfo = arguments.get<0, const neurala::dto::CameraInfo>();
				const auto& cameraOptions = arguments.get<1, const neurala::Options>();

				p = new Source(cameraInfo, cameraOptions);
			}
			catch (const std::exception& e)
			{
				error(e.what());
			}

			return p;
		}

		void
		Source::destroy(void* p)
		{
			delete static_cast<VideoSource*>(p);
		}
	} // namespace PythonCamera
} // namespace neurala

