/*
 * Copyright Neurala Inc. 2013-2025
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

#ifndef NEURALA_CPPCOMPAT_TYPETRAITS_H
#define NEURALA_CPPCOMPAT_TYPETRAITS_H

#if __cplusplus <= 201402L

#include <functional>
#include <type_traits>

#include "neurala/exports.h"

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
{ };

template<class T>
struct conjunction<T> : public T
{ };

template<class T, class... Ts>
struct conjunction<T, Ts...> : public std::conditional<bool(T::value), conjunction<Ts...>, T>::type
{ };

/**
 * @brief Logical OR of types @p Ts.
 *
 * Backported implementation from C++17.
 */
template<class... Ts>
struct disjunction : public std::false_type
{ };

template<class T>
struct disjunction<T> : public T
{ };

template<class T, class... Ts>
struct disjunction<T, Ts...> : public std::conditional<bool(T::value), T, disjunction<Ts...>>::type
{ };

/**
 * @brief Determines whether type @p F can be invoked with the arguments @p Args
 *
 * Backported implementation from C++17.
 */
template<class F, class... Args>
struct is_invocable
 : std::is_constructible<std::function<void(Args...)>,
                         std::reference_wrapper<typename std::remove_reference<F>::type>>
{ };

/**
 * @brief Determines whether @p F can be invoked with the arguments @p Args to yield a result that
 * is convertible to @p R.
 */
template<class R, class F, class... Args>
struct is_invocable_r
 : std::is_constructible<std::function<R(Args...)>,
                         std::reference_wrapper<typename std::remove_reference<F>::type>>
{ };

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
