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
	constexpr bool operator()(const T& x, const T& y) const noexcept
	{
		return x == y;
	}
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
		return std::abs(x - y) <= std::max(absoluteTolerance, relativeTolerance * (std::max(std::abs(x), std::abs(y))));
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
	constexpr bool operator()(const T& x, const T& y) const noexcept
	{
		return x >= y;
	}
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
