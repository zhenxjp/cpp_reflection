#pragma once

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