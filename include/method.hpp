#pragma once

#include "args.hpp"
using namespace std;


class method
{
public:
	template <typename... A>
	value call(void* real,A... arg)
	{
		return call_ex(real,args(arg...));
	}

	virtual value call_ex(void* real, args arg) = 0;
};



template <typename C, typename RET, typename F, typename... A>
class method_impl :public method
{
public:
	method_impl(F func)
	{
		func_ = func;
	}
	virtual value call_ex(void* real, args arg)
	{
		C* c = (C*)real;

		return call_impl(c, arg, std::make_index_sequence<sizeof...(A)>());
	}

	template <size_t... Is>
	value call_impl(C* c,const args& args, std::index_sequence<Is...>)
	{
		RET ret = ((*c).*func_)(convert_arg<A>(args, Is)...);
		return value(ret);
	}

	F func_;
};