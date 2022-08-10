#pragma once

#include "args.hpp"
#include "func_trait.hpp"
using namespace std;



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

template <class FucInfo,typename F>
class method_impl :public method
{
public:
	typedef typename FucInfo::ClsType ClsType;
	typedef typename FucInfo::ReturnType RetType;

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
		if constexpr  (std::is_void_v<RetType>)
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