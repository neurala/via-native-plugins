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

#ifndef NEURALA_PLUGIN_PLUGIN_ARGUMENTS_H
#define NEURALA_PLUGIN_PLUGIN_ARGUMENTS_H

#include <utility>
#include <vector>

#include "neurala/error/Exception.h"
#include "neurala/utils/detail/AnyRef.h"

namespace neurala
{
/**
 * @brief Arguments for plugin instance creation.
 *
 * It type erases all arguments passed to it, but performs type checking at retrieval to avoid
 * incorrect casts.
 */
class PluginArguments
{
	std::vector<AnyRef> m_args;

public:
	/**
	 * @brief Creates a new @ref PluginArguments object that type-erases @p t.
	 */
	template<class... T>
	explicit PluginArguments(T&... t) : m_args({t...})
	{}

	/**
	 * @brief Returns if this object has no arguments.
	 */
	bool empty() const noexcept { return m_args.empty(); }

	/**
	 * @brief Returns the number of arguments stored in this object.
	 */
	std::size_t size() const noexcept { return m_args.size(); }

	/**
	 * @brief Returns if the @p I -th argument is of type @p T.
	 */
	template<std::size_t I, class T>
	bool isOfType() const
	{
		NEURALA_GUARD(I < m_args.size(), "Argument index out-of-bounds");
		return m_args[I].isOfType<T>();
	}

	/**
	 * @brief Returns the @p I -th argument, cast to type @p T.
	 */
	template<std::size_t I, class T>
	T& get() const
	{
		NEURALA_GUARD(I < m_args.size(), "Argument index out-of-bounds");
		return m_args[I].get<T>();
	}
};

} // namespace neurala

#endif // NEURALA_PLUGIN_PLUGIN_ARGUMENTS_H
