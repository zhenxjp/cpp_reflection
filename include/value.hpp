#pragma once

#include <any>
using namespace std;

class value
{
public:
	template <typename T>
	value(const T& val)
	{
		val_ = val;
	}

	value() {}

	template <typename T>
	T to()const
	{
		return std::any_cast<T>(val_);
	}
public:
	any val_;
};