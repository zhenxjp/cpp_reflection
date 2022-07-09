#pragma once

#include <any>
#include <vector>
using namespace std;

class args {
public:
	template <typename... V>
	args(V&&... arg_list)
		: args(std::initializer_list<any>({ std::forward<V>(arg_list)... }))
	{
	}

	args(std::initializer_list<any> il)
	{
		args_ = il;
	}

	const any& operator [] (size_t index) const
	{
		return args_[index];
	}

	std::vector<any> args_; // List of the values
};

template <typename T>
static  T convert_arg(const args& args, size_t index)
{
	return std::any_cast<T>(args[index]);
}