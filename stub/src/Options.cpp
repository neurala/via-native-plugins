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

#include <unordered_map>
#include <utility>
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

void
Options::ImplDelete::operator()(Options::Impl* impl) const
{
	delete impl;
}

Options::Options() : m_impl{new Impl{}} { }

Options::Options(const std::string& name, double value) : Options()
{
	add(name, value);
}

Options::Options(const std::string& name, const std::string& value) : Options()
{
	add(name, value);
}

Options::Options(const Options& other) : m_impl{new Impl{*other.m_impl}} { }

Options&
Options::operator=(const Options& other)
{
	*m_impl = *other.m_impl;
	return *this;
}

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

Options&
Options::add(const std::string& name, const char* value)
{
	m_impl->add(name, std::string{value});
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

template<typename T>
T
Options::as(const std::string& key, const T& defaultValue) const
{
	const T* ptr = m_impl->tryAs<T>(key);
	return ptr == nullptr ? defaultValue : *ptr;
}

bool
Options::asBool(const std::string& key, bool defaultValue) const
{
	return as(key, defaultValue);
}

std::int64_t
Options::asInt(const std::string& key, std::int64_t defaultValue) const
{
	return as(key, defaultValue);
}

double
Options::asDouble(const std::string& key, double defaultValue) const
{
	return as(key, defaultValue);
}

std::string
Options::asString(const std::string& key, const std::string& defaultValue) const
{
	return as(key, defaultValue);
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
