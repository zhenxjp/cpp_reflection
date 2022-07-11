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
private:
	virtual value call_ex(void* real, args arg) = 0;
};

template <typename F>
class method_impl :public method
{
public:
	typedef FunctionDetails<F> FucInfo;
	typedef FucInfo::ClsType ClsType;

	method_impl(F func):func_(func){}
private:
	virtual value call_ex(void* real, args arg)
	{
		return call_impl((ClsType*)real, arg, std::make_index_sequence<FucInfo::ArgCnt>());
	}

	template <size_t... Is>
	value call_impl(ClsType* c,const args& args, std::index_sequence<Is...>)
	{
		// 萃取参数类型
#define ARG(idx)	typename FucInfo::template args<idx>::type

		ClsType& obj = *c;
		if constexpr  (std::is_void_v<FucInfo::ReturnType>)
		{
			// for return void
			(obj.*func_)(args.to<ARG(Is)>(Is)...);
			return value();
		}
		else {
			// for return no void
			value ret = (obj.*func_)(args.to<ARG(Is)>(Is)...);
			return value(ret);
		}
	}

	F func_ = nullptr;
};