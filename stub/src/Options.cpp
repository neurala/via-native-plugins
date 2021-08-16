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

#include "neurala/utils/Options.h"

namespace neurala
{
/// @brief Implementation of @ref Options.
class Options::Impl
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

Options::Options() : m_impl{std::make_unique<Impl>()} { }

Options::Options(const std::string& name, double value) : Options()
{
	add(name, value);
}

Options::Options(const std::string& name, const std::string& value) : Options()
{
	add(name, value);
}

Options::Options(const Options& other) : m_impl{std::make_unique<Impl>(*other.m_impl)} { }

Options::Options(Options&&) noexcept = default;

Options::~Options() = default;

Options&
Options::operator=(const Options& other)
{
	m_impl = std::make_unique<Impl>(*(other.m_impl));
	return *this;
}

Options& Options::operator=(Options&&) noexcept = default;

bool
Options::empty() const noexcept
{
	return m_impl->empty();
}

Options::size_type
Options::size() const noexcept
{
	return m_impl->size();
}

Options&
Options::add(const std::string& name, bool value)
{
	m_impl->add(name, value);
	return *this;
}

Options&
Options::add(const std::string& name, std::int64_t value)
{
	m_impl->add(name, value);
	return *this;
}

Options&
Options::add(const std::string& name, double value)
{
	m_impl->add(name, value);
	return *this;
}

Options&
Options::add(const std::string& name, const std::string& value)
{
	m_impl->add(name, value);
	return *this;
}

bool
Options::asBool(const std::string& name) const
{
	return m_impl->as<bool>(name);
}

std::int64_t
Options::asInt(const std::string& name) const
{
	return m_impl->as<std::int64_t>(name);
}

double
Options::asDouble(const std::string& name) const
{
	return m_impl->as<double>(name);
}

const std::string&
Options::asString(const std::string& name) const
{
	return m_impl->as<std::string>(name);
}

const bool*
Options::tryAsBool(const std::string& name) const
{
	return m_impl->tryAs<bool>(name);
}

const std::int64_t*
Options::tryAsInt(const std::string& name) const
{
	return m_impl->tryAs<std::int64_t>(name);
}

const double*
Options::tryAsDouble(const std::string& name) const
{
	return m_impl->tryAs<double>(name);
}

const std::string*
Options::tryAsString(const std::string& name) const
{
	return m_impl->tryAs<std::string>(name);
}

Options&
Options::mergeIn(const Options& other)
{
	m_impl->mergeIn(*(other.m_impl));
	return *this;
}

Options&
Options::mergeIn(Options&& other)
{
	m_impl->mergeIn(std::move(*(other.m_impl)));
	return *this;
}

Options&
Options::operator|=(const Options& other)
{
	return mergeIn(other);
}

Options&
Options::operator|=(Options&& other)
{
	return mergeIn(std::move(other));
}

Options
operator|(Options x, const Options& y)
{
	x.mergeIn(y);
	return x;
}

Options
operator|(Options x, Options&& y)
{
	x.mergeIn(std::move(y));
	return x;
}

std::ostream&
operator<<(std::ostream& os, const Options& option)
{
	return os << *(option.m_impl);
}

} // namespace neurala
