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

#ifndef NEURALA_UTILS_GET_LINE_H
#define NEURALA_UTILS_GET_LINE_H

#include <istream>
#include <string>

#include "neurala/exports.h"

namespace neurala
{
/**
 * @brief Reads the contents of @p source until the next @p delim character, and places them into
 * the @p line string.
 *
 * @param source source to read from
 * @param line   output string to write to
 * @param delim  delimiter character
 *
 * @return the input @p source
 */
NEURALA_PUBLIC std::istream& getLine(std::istream& source, std::string& line, char delim);

/**
 * @brief Reads the contents of @p source until the next end of line character (<tt>"\n"</tt> or
 * <tt>"\r\n"</tt>) and places them into the @p line string.
 *
 * @param source source to read from
 * @param line   output string to write to
 *
 * @return the input @p source
 */
NEURALA_PUBLIC std::istream& getLine(std::istream& source, std::string& line);

} // namespace neurala

#endif // NEURALA_UTILS_GET_LINE_H
