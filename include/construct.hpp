#pragma once

#include "args.hpp"
using namespace std;


class construct
{
public:
	template <typename... A>
	void* create(A... arg)
	{
		return create_ex(args(arg...));
	}

	virtual void* create_ex(args arg) = 0;
};

template <typename C, typename... A>
class construct_impl :public construct
{
public:
	virtual void* create_ex(args arg)
	{
		return create_impl(arg, std::make_index_sequence<sizeof...(A)>());
	}

	template <size_t... Is>
	void* create_impl(const args& args, std::index_sequence<Is...>)
	{
		return new C(convert_arg<A>(args, Is)...);
	}
};