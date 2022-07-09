#pragma once
#include "value.hpp"
#include <vector>
using namespace std;

class args {
public:
	template <typename... V>
	args(V&&... arg_list)
		: args(std::initializer_list<value>({ std::forward<V>(arg_list)... }))
	{
	}

	args(std::initializer_list<value> il)
	{
		args_ = il;
	}

	const value& operator [] (size_t index) const
	{
		return args_[index];
	}

	std::vector<value> args_; // List of the values
};

template <typename T>
static  T convert_arg(const args& args, size_t index)
{
	return args[index].to<T>();
}