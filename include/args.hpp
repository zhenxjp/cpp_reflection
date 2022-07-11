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

	template <typename T>
	T to(size_t index)const
	{
		return args_[index].to<T>();
	}

	std::vector<value> args_; // List of the values
};