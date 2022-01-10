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
#ifndef NEURALA_PYTHON_PLUGIN_H
#define NEURALA_PYTHON_PLUGIN_H

#include <iostream>
#include <thread>

#include "Python.h"

#ifndef NDEBUG
#define DEBUG(x) std::cout << "[thread 0x" << std::hex << std::this_thread::get_id() << std::dec << "]: " << x << "\n";
#else
#define DEBUG(x) do {} while (0);
#endif

// These macros acquire/release the Global Interpreter Lock and can be called any number of times on a single thread
// as long as they are used in pairs.
#define ACQUIRE_GIL \
			PyGILState_STATE gil_;  \
			gil_ = PyGILState_Ensure();

#define RELEASE_GIL \
			PyGILState_Release(gil_);


namespace neurala {
// equivalent to callableSpec.partition(":") in Python, returns the resulting tuple `(before, sep, after)` or NULL on error
// caller owns a reference to the returned tuple
PyObject*
parseCallableSpec(std::string callableSpec)
{
    PyObject *args, *result, *spec = NULL;
    ACQUIRE_GIL;

    spec = PyUnicode_FromString(callableSpec.c_str());

    args = PyTuple_Pack(1, PyUnicode_FromString(":"));
    result = PyObject_Call(PyObject_GetAttrString(spec, "partition"), args, NULL);
    Py_XDECREF(args);

    RELEASE_GIL;
    return result;
}


// given a specification like mod.pkg:obj, import mod.pkg and return obj, or NULL if an error occurs
// caller owns a reference to the returned callable object
PyObject*
objFromSpec(std::string specstr)
{
    PyObject *spec, *mod, *moduleName, *objName, *result = NULL;
    ACQUIRE_GIL;

    spec = parseCallableSpec(specstr);
    moduleName = PyTuple_GET_ITEM(spec, 0);  // borrowed reference
    objName = PyTuple_GET_ITEM(spec, 2);  // borrowed reference

    DEBUG("moduleName=" << PyUnicode_AS_DATA(moduleName) << ", " <<
          "objName=" << PyUnicode_AS_DATA(objName) << "\n");

    // NOCOMMIT
    // if (objName == "")
    // {
    //     objName = "some_sane_default";
    // }

    mod = PyImport_Import(moduleName);
    if (mod == NULL) {
        DEBUG("can't import module");
        goto cleanup;
    }
    result = PyObject_GetAttr(mod, objName);
    if (result == NULL) {
        DEBUG("can't retrieve object");
        Py_XDECREF(mod);
        goto cleanup;
    }

    // check if it's callable?

cleanup:
    Py_XDECREF(mod);
    RELEASE_GIL;
    return result;
}


/*
Returns 0 on success, -1 on failure

NOTE: caller is responsible for acquiring the GIL before calling this function
NOTE: caller is responsible for calling PyBuffer_Release(buf) when the buffer is no longer being used, so that obj can be garbage collected
*/
int
bufferFromPyObject(PyObject* obj, Py_buffer* buf, int flags = 0)
{
    int err = 0;

    /* If the object (e.g. io.BytesIO) defines getbuffer(), we should call it first to get the right object to pass to PyObject_GetBuffer() */
    if (PyObject_HasAttrString(obj, "getbuffer") == 1)
    {
        obj = PyObject_CallMethod(obj, "getbuffer", NULL);
        if (obj == NULL) {
            DEBUG("Error occurred when calling getbuffer() on Python object\n");
            err = -1;
        }
    }

    if (!PyObject_CheckBuffer(obj))
    {
        DEBUG("Python object does not support the buffer protocol\n");
        err = -1;
    }

    if (PyObject_GetBuffer(obj, buf, flags) != 0)
    {
        DEBUG("Error occurred when calling PyObject_GetBuffer()\n");
        err = -1;
    }

    return err;
}


void
initializePython(std::wstring programName)
{
	Py_SetProgramName(programName.c_str());

	if (!Py_IsInitialized())
	{
		DEBUG("Initializing Python\n");
        // NOTE:20220106:jgerity:the argument prevents signal handler registration
		Py_InitializeEx(0);
	} else {
		DEBUG("see an already-initialized Python\n");
	}

	// NOTE:20220106:jgerity:this is necessary for CPython 3.6, but not later versions
	if (!PyEval_ThreadsInitialized())
	{
		PyEval_InitThreads();
	}

    // NOTE:20220105:jgerity:If we hold the GIL after calling Py_Initialize() (i.e. we called it first), we need to
    // release it so that other threads can acquire it.
	if (PyGILState_Check())
	{
		PyEval_SaveThread();
	}
}

} // namespace neurala

#endif // NEURALA_PYTHON_PLUGIN_H