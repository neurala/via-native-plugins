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

#ifndef NEURALA_CPPCOMPAT_TYPETRAITS_H
#define NEURALA_CPPCOMPAT_TYPETRAITS_H

#if __cplusplus <= 201402L

#include <functional>
#include <type_traits>

namespace std17
{
namespace detail
{
template<class... Ts>
struct makeVoid
{
	using type = void;
};

} // namespace detail

template<class... Ts>
using void_t = typename detail::makeVoid<Ts...>::type;

/**
 * @brief Logical AND of types @p Ts.
 *
 * Backported implementation from C++17.
 */
template<class... Ts>
struct conjunction : std::true_type
{};

template<class T>
struct conjunction<T> : public T
{};

template<class T, class... Ts>
struct conjunction<T, Ts...> : public std::conditional<bool(T::value), conjunction<Ts...>, T>::type
{};

/**
 * @brief Logical OR of types @p Ts.
 *
 * Backported implementation from C++17.
 */
template<class... Ts>
struct disjunction : public std::false_type
{};

template<class T>
struct disjunction<T> : public T
{};

template<class T, class... Ts>
struct disjunction<T, Ts...> : public std::conditional<bool(T::value), T, disjunction<Ts...>>::type
{};

/**
 * @brief Determines whether type @p F can be invoked with the arguments @p Args
 *
 * Backported implementation from C++17.
 */
template<class F, class... Args>
struct is_invocable
 : std::is_constructible<std::function<void(Args...)>,
                         std::reference_wrapper<typename std::remove_reference<F>::type>>
{};

/**
 * @brief Determines whether @p F can be invoked with the arguments @p Args to yield a result that
 * is convertible to @p R.
 */
template<class R, class F, class... Args>
struct is_invocable_r
 : std::is_constructible<std::function<R(Args...)>,
                         std::reference_wrapper<typename std::remove_reference<F>::type>>
{};

} // namespace std17

#else

#include <type_traits>

namespace std17
{
using std::conjunction;
using std::disjunction;
using std::is_invocable;
using std::is_invocable_r;
using std::void_t;

} // namespace std17

#endif // __cplusplus

#endif // NEURALA_CPPCOMPAT_TYPETRAITS_H
