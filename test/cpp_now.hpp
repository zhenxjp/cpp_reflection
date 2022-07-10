#pragma once
#include <iostream>
#include <type_traits>
#include "method.hpp"
using namespace std;

// 获取类型信息
template <class T>
static void test_typeid(const T &t)
{
	const type_info& type = typeid(t);
	printf("name=%s\n", type.name());
}


static void test_simple()
{
	test_typeid((int)1);	// 简单类型
	test_typeid("ccc");		// 简单类型
}

// 父类
class father
{
public:
	virtual void f() {};
};
// 子类
class son : public father
{
public:
	virtual void f() {};

};
static void test_Derived()
{
	father* father_ptr = new father;
	test_typeid(*father_ptr);	// 指向父类的父类指针

	father* son_ptr = new son;
	test_typeid(*son_ptr);		// 指向子类的父类指针
}

template <typename... A>
static int get_arg_cnt(A... arg)
{
	int cnt = sizeof...(A);
	printf("cnt=%d\n", cnt);
	return cnt;
}

template <typename... A>
static int get_arg_cnt2(A... arg)
{
	int cnt = tuple_size<tuple<A...>>::value;
	printf("cnt=%d\n", cnt);
	return cnt;
}
#include <any>
void test_any()
{
	std::any a = 1;

	son s;
	a = s;

}


class XXXXXX
{
public:
	XXXXXX()
	{
		printf("xxx");
	}
	XXXXXX(int a1,int a2)
	{
		xx1 = a1;
		xx2 = a2;
	}

	int f(int val)
	{
		return xx1 + xx2 + val;
	}

	string get_str(const char* p)
	{
		return xx3 + p;
	}

	int xx1;
	int xx2;

	std::string xx3;
};

void test_func()
{
	XXXXXX xx(10, 20);
	auto func = &XXXXXX::f;
	(xx.*func)(100);
}


#include "construct.hpp"

void test_what()
{
	construct* c = new construct_impl<XXXXXX,int,int>();
	c->create(1,2);
}

void test_method()
{

	{
		XXXXXX xx(10, 20);

		method* m = new method_impl(&XXXXXX::f);
		int ret = m->call(&xx, 1000).to<int>();
	}
	{
		XXXXXX xx(10, 20);

		xx.xx3 = "1111";
		method* m = new method_impl(&XXXXXX::get_str);
		string  ret = m->call(&xx, "ccc").to<string>();
	}

}

void test_cpp_now()
{
	test_method();
	test_func();
	test_what();
	test_any();
	get_arg_cnt(1, "ccc", string("ccc"));
	get_arg_cnt2(1,"ccc",string("ccc"));

	test_simple();
	test_Derived();
}