/// Copyright (c) Vito Domenico Tagliente

#pragma once

#include <initializer_list>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "number.h"

namespace json
{
	class value
	{
	public:

		enum class Type : uint8_t
		{
			Array	= 0,
			Bool	= 1,
			Null	= 2,
			Number	= 3,
			Object	= 4,
			String	= 5
		};

		using array_t = std::vector<value>;
		using object_t = std::unordered_map<std::string, value>;

		value()
			: m_type(Type::Null)
			, m_value()
		{}

		value(const bool value)
			: m_type(Type::Bool)
			, m_value(value)
		{}

		value(const int value)
			: m_type(Type::Number)
			, m_value(number(value))
		{}

		value(const float value)
			: m_type(Type::Number)
			, m_value(number(value))
		{}

		value(const double value)
			: m_type(Type::Number)
			, m_value(number(value))
		{}

		value(const std::string& value)
			: m_type(Type::String)
			, m_value(value)
		{}

		value(const char* value)
			: m_type(Type::String)
			, m_value(std::string(value))
		{}

		value(const array_t& value)
			: m_type(Type::Array)
			, m_value(value)
		{}

		value(const object_t& value)
			: m_type(Type::Object)
			, m_value(value)
		{}

		value(const number& value)
			: m_type(Type::Number)
			, m_value(value)
		{}

		value(const std::initializer_list<value>& values)
			: m_type(Type::Array)
			, m_value(array_t{ values })
		{}

		Type type() const { return m_type; }

		bool is_array() const { return m_type == Type::Array; }
		bool is_bool() const { return m_type == Type::Bool; }
		bool is_null() const { return m_type == Type::Null; }
		bool is_number() const { return m_type == Type::Number; }
		bool is_object() const { return m_type == Type::Object; }
		bool is_string() const { return m_type == Type::String; }

		bool& as_bool() { return std::get<bool>(m_value); }
		number& as_number() { return std::get<number>(m_value); }
		array_t& as_array() { return std::get<array_t>(m_value); }
		object_t& as_object() { return std::get<object_t>(m_value); }
		std::string& as_string() { return std::get<std::string>(m_value); }

		bool as_bool() const { return std::get<bool>(m_value); }
		const number& as_number() const { return std::get<number>(m_value); }
		const array_t& as_array() const { return std::get<array_t>(m_value); }
		const object_t& as_object() const { return std::get<object_t>(m_value); }
		const std::string& as_string() const { return std::get<std::string>(m_value); }

		bool operator== (const value& other) const
		{
			return m_type == other.m_type
				&& m_value == other.m_value;
		}

		bool operator!= (const value& other) const
		{
			return m_type != other.m_type
				|| m_value != other.m_value;
		}

		value& operator= (const bool value)	
		{ 
			m_type = Type::Bool; 
			m_value = value;	
			return *this; 
		}
		
		value& operator= (const int value) 
		{ 
			m_type = Type::Number; 
			m_value = number(value); 
			return *this; 
		}
		
		value& operator= (const float value) 
		{ 
			m_type = Type::Number; 
			m_value = number(value); 
			return *this; 
		}
		
		value& operator= (const double value) 
		{ 
			m_type = Type::Number; 
			m_value = number(value); 
			return *this; 
		}
		
		value& operator= (const array_t& value) 
		{ 
			m_type = Type::Array; 
			m_value = value; 
			return *this; 
		}
		
		value& operator= (const std::string& value) 
		{ 
			m_type = Type::String; 
			m_value = value; 
			return *this; 
		}
		
		value& operator= (const object_t& value) 
		{ 
			m_type = Type::Object; 
			m_value = value; 
			return *this; 
		}

		// Prefix increment operator
		value& operator++ () 
		{
			if (is_number())
				as_number()++;
			return *this;
		}

		// Postfix increment operator
		value operator++ (int)
		{
			if (is_number())
			{
				value temp(as_number());
				as_number()++;
				return temp;
			}
			return *this;
		}

		// Prefix decrement operator
		value& operator-- ()
		{
			if (is_number())
				as_number()--;
			return *this;
		}

		// Postfix decrement operator
		value operator-- (int)
		{
			if (is_number())
			{
				value temp(as_number());
				as_number()--;
				return temp;
			}
			return *this;
		}

		template<typename T, typename TEnable = std::enable_if_t<std::is_arithmetic<T>::value>>
		value& operator += (const T value)
		{ 
			if (m_type == Type::Number) 
				std::get<number>(m_value) += value; 
			return *this; 
		}
		
		template<typename T, typename TEnable = std::enable_if_t<std::is_arithmetic<T>::value>>
		value& operator -= (const T value)
		{ 
			if (m_type == Type::Number) 
				std::get<number>(m_value) -= value; 
			return *this; 
		}
		
		template<typename T, typename TEnable = std::enable_if_t<std::is_arithmetic<T>::value>>
		value& operator *= (const T value)
		{ 
			if (m_type == Type::Number) 
				std::get<number>(m_value) *= value; 
			return *this; 
		}
		
		template<typename T, typename TEnable = std::enable_if_t<std::is_arithmetic<T>::value>>
		value& operator /= (const T value)
		{ 
			if (m_type == Type::Number) 
				std::get<number>(m_value) /= value; 
			return *this; 
		}

		template<typename T, typename TEnable = std::enable_if_t<std::is_arithmetic<T>::value>>
		value operator+ (const T value) const
		{
			if (m_type == Type::Number) 
				return value(as_number() + value);
			return *this;
		}

		template<typename T, typename TEnable = std::enable_if_t<std::is_arithmetic<T>::value>>
		value operator- (const T value) const
		{
			if (m_type == Type::Number)
				return value(as_number() - value);
			return *this;
		}

		template<typename T, typename TEnable = std::enable_if_t<std::is_arithmetic<T>::value>>
		value operator* (const T value) const
		{
			if (m_type == Type::Number)
				return value(as_number() * value);
			return *this;
		}

		template<typename T, typename TEnable = std::enable_if_t<std::is_arithmetic<T>::value>>
		value operator/ (const T value) const
		{
			if (m_type == Type::Number)
				return value(as_number() / value);
			return *this;
		}

		value& operator[](size_t index)
		{
			return as_array().at(index);
		}

		const value& operator[](size_t index) const
		{
			return as_array().at(index);
		}

		value& operator[](const char* key)
		{
			return as_object()[std::string(key)];
		}

		value& operator[](const std::string& key)
		{
			return as_object()[key];
		}

		const value& operator[](const char* key) const
		{
			return as_object().at(std::string(key));
		}

		const value& operator[](const std::string& key) const
		{
			return as_object().at(key);
		}

	private:

		Type m_type;
		std::variant<array_t, bool, number, object_t, std::string> m_value;

	};
}