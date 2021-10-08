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

#ifndef NEURALA_ALGORITHMS_MATH_UTILS_H
#define NEURALA_ALGORITHMS_MATH_UTILS_H

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <type_traits>

namespace neurala
{
/**
 * @brief Returns if @p x and @p y are equal.
 */
template<class T, class = void>
struct ApproxEqualTo
{
	constexpr bool operator()(const T& x, const T& y) const noexcept { return x == y; }
};

/**
 * @brief Specialization of @ref ApproxEqualTo for floating point types.
 *
 * This function object does an approximate comparison of floating points that ignores the machine
 * epsilon.
 */
template<class T>
struct ApproxEqualTo<T, std::enable_if_t<std::is_floating_point<T>::value>>
{
	constexpr bool operator()(const T& x, const T& y) const noexcept
	{
		if (std::isunordered(x, y))
		{
			// no comparison between NaNs
			return false;
		}

		if (x == y)
		{
			// short-circuiting
			return true;
		}

		constexpr auto absoluteTolerance = std::numeric_limits<T>::epsilon();
		constexpr auto relativeTolerance = std::numeric_limits<T>::epsilon();
		return std::abs(x - y)
		       <= std::max(absoluteTolerance, relativeTolerance * (std::max(std::abs(x), std::abs(y))));
	}
};

/**
 * @brief Returns if @p x and @p y are approximately equal.
 *
 * @param x operation LHS
 * @param y operation RHS
 *
 * @return @c true if <tt>x == y</tt> within machine error
 */
template<class T>
constexpr bool
isApproxEqual(const T& x, const T& y) noexcept
{
	return ApproxEqualTo<T>{}(x, y);
}

/**
 * @brief Returns if all elements in range @p first1, @p last2 are approximately equal to elements
 * in [@pfirst2, @pfirst2 + @plast1- @pfirst1).
 *
 * @param first1 iterator start for first data structure
 * @param last1 iterator end for first data structure
 * @param first2 iterator start first data structure
 *
 * @return @c true if all elements in range are equal within machine error
 */
template<class It1, class It2>
constexpr bool
approxEqual(It1 firstIt1, It1 lastIt1, It2 firstIt2)
{
	for (; firstIt1 != lastIt1; ++firstIt1, ++firstIt2)
	{
		if (!isApproxEqual(*firstIt1, *firstIt2))
		{
			return false;
		}
	}
	return true;
}

/**
 * @brief Returns if @p x is greater or equal to @p y.
 */
template<class T, class = void>
struct ApproxGreaterOrEqualTo
{
	constexpr bool operator()(const T& x, const T& y) const noexcept { return x >= y; }
};

/**
 * @brief Specialization of @ref ApproxGreaterOrEqualTo for floating point types.
 */
template<class T>
struct ApproxGreaterOrEqualTo<T, std::enable_if_t<std::is_floating_point<T>::value>>
{
	constexpr bool operator()(const T& x, const T& y) const noexcept
	{
		return isApproxEqual(x, y) || x > y;
	}
};

/**
 * @brief Returns if @p x is approximately greater or equal to @p y.
 *
 * @param x operation LHS
 * @param y operation RHS
 *
 * @return @c true if <tt>x >= y</tt> within machine error
 */
template<class T>
constexpr bool
isApproxGreaterOrEqual(const T& x, const T& y) noexcept
{
	return ApproxGreaterOrEqualTo<T>{}(x, y);
}

/**
 * @brief checks if @p a is greater than 0 and less than @p b, in a single conditional
 *        check, by casting to unsigned.
 *
 * @param a value to check whether it is greater than 0 and less than @p b
 * @param b value to check whether @p a is less than.
 *
 * @return @c true if <tt>a > 0 && a < b</tt>
 */
template<class Size>
constexpr bool
isGreaterThanOrEqualZeroAndLessThan(Size a, std::make_unsigned_t<Size> b) noexcept
{
	static_assert(std::is_signed<Size>::value && std::is_integral<Size>::value,
	              "Only valid for integral signed types");
	return static_cast<std::make_unsigned_t<Size>>(a) < b;
}

/**
 * @brief Returns the upper bound for division of @p a and @p b.
 */
constexpr int
divUp(int a, int b) noexcept
{
	return (a % b != 0) ? (a / b + 1) : (a / b);
}

} // namespace neurala

#endif // NEURALA_ALGORITHMS_MATH_UTILS_H
