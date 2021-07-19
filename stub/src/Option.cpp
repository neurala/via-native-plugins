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

#include <unordered_map>
#include <variant>

#include "neurala/utils/Option.h"

namespace neurala
{

/// @brief Implementation of @ref Option.
class Option::Impl
{
	using OptionValueType = std::variant<bool, std::int64_t, double, std::string>;

	std::unordered_map<std::string, OptionValueType> m_options;

public:
	bool empty() const noexcept { return m_options.empty(); }

	size_type size() const noexcept { return m_options.size(); }

	template<class T>
	void add(const std::string& name, const T& value)
	{
		m_options.emplace(name, OptionValueType(value));
	}

	template<class T>
	const T& as(const std::string& name) const
	{
		return std::get<T>(m_options.at(name));
	}

	template<class T>
	const T* tryAs(const std::string& name) const
	{
		auto it = m_options.find(name);
		if (it == m_options.cend())
		{
			return nullptr;
		}
		try
		{
			return &(std::get<T>(it->second));
		}
		catch (const std::bad_variant_access&)
		{
			return nullptr;
		}
	}

	void mergeIn(const Impl& other)
	{
		for (auto& option : other.m_options)
		{
			m_options[option.first] = option.second;
		}
	}

	void mergeIn(Impl&& other)
	{
		auto tmp = std::move(other);
		for (auto& option : tmp.m_options)
		{
			m_options[option.first] = std::move(option.second);
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const Impl& option)
	{
		for (auto& o : option.m_options)
		{
			os << o.first << '=';
			std::visit([&os](auto& t) { os << t; }, o.second);
			os << ' ';
		}
		return os;
	}
};

Option::Option() : m_impl{std::make_unique<Impl>()} { }

Option::Option(const std::string& name, double value) : Option()
{
	add(name, value);
}

Option::Option(const std::string& name, const std::string& value) : Option()
{
	add(name, value);
}

Option::Option(const Option& other) : m_impl{std::make_unique<Impl>(*other.m_impl)} { }

Option::Option(Option&&) noexcept = default;

Option::~Option() = default;

Option&
Option::operator=(const Option& other)
{
	m_impl = std::make_unique<Impl>(*(other.m_impl));
	return *this;
}

Option& Option::operator=(Option&&) noexcept = default;

bool
Option::empty() const noexcept
{
	return m_impl->empty();
}

Option::size_type
Option::size() const noexcept
{
	return m_impl->size();
}

Option&
Option::add(const std::string& name, bool value)
{
	m_impl->add(name, value);
	return *this;
}

Option&
Option::add(const std::string& name, std::int64_t value)
{
	m_impl->add(name, value);
	return *this;
}

Option&
Option::add(const std::string& name, double value)
{
	m_impl->add(name, value);
	return *this;
}

Option&
Option::add(const std::string& name, const std::string& value)
{
	m_impl->add(name, value);
	return *this;
}

bool
Option::asBool(const std::string& name) const
{
	return m_impl->as<bool>(name);
}

std::int64_t
Option::asInt(const std::string& name) const
{
	return m_impl->as<std::int64_t>(name);
}

double
Option::asDouble(const std::string& name) const
{
	return m_impl->as<double>(name);
}

const std::string&
Option::asString(const std::string& name) const
{
	return m_impl->as<std::string>(name);
}

const bool*
Option::tryAsBool(const std::string& name) const
{
	return m_impl->tryAs<bool>(name);
}

const std::int64_t*
Option::tryAsInt(const std::string& name) const
{
	return m_impl->tryAs<std::int64_t>(name);
}

const double*
Option::tryAsDouble(const std::string& name) const
{
	return m_impl->tryAs<double>(name);
}

const std::string*
Option::tryAsString(const std::string& name) const
{
	return m_impl->tryAs<std::string>(name);
}

Option&
Option::mergeIn(const Option& other)
{
	m_impl->mergeIn(*(other.m_impl));
	return *this;
}

Option&
Option::mergeIn(Option&& other)
{
	m_impl->mergeIn(std::move(*(other.m_impl)));
	return *this;
}

Option&
Option::operator|=(const Option& other)
{
	return mergeIn(other);
}

Option&
Option::operator|=(Option&& other)
{
	return mergeIn(std::move(other));
}

Option
operator|(Option x, const Option& y)
{
	x.mergeIn(y);
	return x;
}

Option
operator|(Option x, Option&& y)
{
	x.mergeIn(std::move(y));
	return x;
}

std::ostream&
operator<<(std::ostream& os, const Option& option)
{
	return os << *(option.m_impl);
}

} // namespace neurala
