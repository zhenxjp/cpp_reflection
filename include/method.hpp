#pragma once

#include "args.hpp"
using namespace std;


template <typename T>
struct FunctionDetails {};

template <typename R, typename C, typename... A>
struct FunctionDetails<R(C::*)(A...)>
{
	typedef R ReturnType;
	typedef C ClsType;
	typedef std::tuple<A...> ParamTypes;

	enum
	{
		ArgCnt = tuple_size<ParamTypes>::value
	};

	template<size_t I>
	struct args
	{
		static_assert(I < ArgCnt, "index is out of range, index must less than sizeof Args");
		using type = typename std::tuple_element<I, ParamTypes>::type;
	};
};

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



template <typename F>
class method_impl :public method
{
public:
	typedef FunctionDetails<F> FucInfo;
	typedef FucInfo::ClsType ClsType;

	method_impl(F func)
	{
		func_ = func;
	}
	virtual value call_ex(void* real, args arg)
	{
		auto* c = (ClsType*)real;
		return call_impl(c, arg, std::make_index_sequence<FucInfo::ArgCnt>());
	}

	template <size_t... Is>
	value call_impl(ClsType* c,const args& args, std::index_sequence<Is...>)
	{
		if constexpr  (std::is_void_v<FucInfo::ReturnType>)
		{
			((*c).*func_)(args[Is].to< typename FucInfo::template args<Is>::type >()...);
			return value();
		}
		else {
			value ret = ((*c).*func_)(args[Is].to< typename FucInfo::template args<Is>::type >()...);
			return value(ret);
		}
	}

	F func_;
};