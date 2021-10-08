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

#ifndef NEURALA_CONFIG_OS_H
#define NEURALA_CONFIG_OS_H

#if !defined(NEURALA_OS_WINDOWS)                                             \
 && (defined(WIN32)       /* Defined by CMake if OS is Windows (any arch) */ \
     || defined(_WINDOWS) /* Defined for NVCC invocations by CMake */        \
     || defined(_WIN32)   /* All 32bits and 64bits Windows builds */         \
     || defined(_WIN64))  /* All 64bits Windows builds */
#define NEURALA_OS_WINDOWS
#endif

#if !defined(NEURALA_OS_APPLE) && defined(__APPLE__) /* Defined by Apple Clang for iOS and macOS \
                                                        both */
#define NEURALA_OS_APPLE
#endif

#endif // NEURALA_CONFIG_OS_H
