#pragma once
#include <iostream>
#include <type_traits>
#include "method.hpp"
using namespace std;

// 获取类型信息
template <class T>
static void test_typeid(const T& t)
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
	XXXXXX(int a1, int a2)
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
	construct* c = new construct_impl<XXXXXX, int, int>();
	c->create(1, 2);
}

#include "v_func.hpp"
void test_virtual_func()
{
	class A
	{
	public:
		virtual void f0() { cout << __FUNCTION__; }
		virtual void f1() { cout << __FUNCTION__; }
		virtual void f2() { cout << __FUNCTION__; }
		virtual void f3() { cout << __FUNCTION__; }
		virtual void f4() { cout << __FUNCTION__; }
		virtual void f5() { cout << __FUNCTION__; }
		virtual void f6() { cout << __FUNCTION__; }
		virtual void f7() { cout << __FUNCTION__; }
		virtual void f8() { cout << __FUNCTION__; }
		virtual void f9() { cout << __FUNCTION__; }
		virtual void f10() { cout << __FUNCTION__; }
		virtual void f11() { cout << __FUNCTION__; }
		virtual void f12() { cout << __FUNCTION__; }
		virtual void f13() { cout << __FUNCTION__; }
		virtual void f14() { cout << __FUNCTION__; }
		virtual void f15() { cout << __FUNCTION__; }
		virtual void f16() { cout << __FUNCTION__; }
		virtual void f17() { cout << __FUNCTION__; }

		int a = 0;
	};

	class B :public A
	{
	public:
		virtual void f0() { cout << __FUNCTION__; }
		virtual void f1() { cout << __FUNCTION__; }
		virtual void f17() { cout << __FUNCTION__; }

		int b = 10;
	};

	auto a_f0 = &A::f0;
	auto a_f1 = &A::f1;
	auto a_f17 = &A::f17;

	auto b_f0 = &B::f0;
	auto b_f1 = &B::f1;
	auto b_f17 = &B::f17;


	A* p = new B;
	p->f0();
	p->f1();
	p->f17();

	auto f2_bb = get_vfunc(p, 0);

	cout << endl;
	cout << "&B::f0 idx=" << get_vfunc_idx(&B::f0) << endl;
	cout << "&B::f1 idx=" << get_vfunc_idx(&B::f1) << endl;
	cout << "&B::f17 idx=" << get_vfunc_idx(&B::f17)<<endl;

	auto B_f17 = get_real_vfunc(&B::f17);
	auto temp = get_vfunc(p, 17);
}

void test_cpp_now()
{
	test_virtual_func();
	//test_method2();
	test_func();
	test_what();
	test_any();
	get_arg_cnt(1, "ccc", string("ccc"));
	get_arg_cnt2(1, "ccc", string("ccc"));

	test_simple();
	test_Derived();
}