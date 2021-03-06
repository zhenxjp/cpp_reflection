#pragma once
#include "class.hpp"
#include <sstream>
using namespace std;


////////////////////////////////////////////////////

class XXX
{
public:
    XXX(int a1, int a2, string a3)
    {
        xx1 = a1;
        xx2 = a2;
        xx3 = a3;
    }

    void show()
    {
        printf("XXX xx1=%d,xx2=%d,xx3=%s\n", xx1, xx2, xx3.c_str());
    }
    string get_state(const char* p, int val)
    {
        stringstream ss;
        ss << "xx1=" << xx1 << ",";
        ss << "xx2=" << xx2 << ",";
        ss << "xx3=" << xx3 << ",";
        ss << "p=" << p << ",";
        ss << "val=" << val << ",";

        return ss.str();
    }

    int xx1;
    int xx2;

    std::string xx3;
};

static void reg()
{
    g_cls_mgr.create("XXX")                    // 注册类
        .add_pro("xx1", &XXX::xx1)                  // 注册类成员1
        .add_pro("xx2", &XXX::xx2)                  // 注册类成员2
        .add_pro("xx3", &XXX::xx3)                  // 注册类成员3
        .add_con<XXX,int,int,string>()              // 注册构造函数
        .add_method("show", &XXX::show)             // 注册类方法1
        .add_method("get_state", &XXX::get_state)   // 注册类方法2
        ;
}

static void test_proper()
{
    // 创建
    cls* c = g_cls_mgr.get_class("XXX");
    obj xx = c->create(110, 119, string("jqka"));

    {
        // 获取
        auto xx1 = xx.get("xx1").to<int>();
        auto xx2 = xx.get("xx2").to<int>();
        auto xx3 = xx.get("xx3").to<string>();

        printf("reflect get1 xx1=%d,xx2=%d,xx3=%s\n", xx1, xx2, xx3.c_str());
    }
    // 设置
    xx.set("xx1", 1000);
    xx.set("xx2", 2000);
    xx.set("xx3", string("2222"));

    {
        // 再次获取
        auto xx1 = xx.get("xx1").to<int>();
        auto xx2 = xx.get("xx2").to<int>();
        auto xx3 = xx.get("xx3").to<string>();
        printf("reflect get2 xx1=%d,xx2=%d,xx3=%s\n", xx1, xx2, xx3.c_str());
    }
}

static void test_create()
{
    cls* c = g_cls_mgr.get_class("XXX");            // 找到class的元信息
    obj o = c->create(110, 119, string("jqka"));    // 创建
    o.to<XXX>()->show();                            // to 转化为实际类型
}

static void test_get_all_proper()
{
    // 创建对象
    XXX x(111, 222, "xxx");

    // 找到class的元信息
    cls* c = g_cls_mgr.get_class("XXX");

    // 遍历属性
    for (auto pro : c->pros_)
    {
        proper* p = pro.second;
        printf("key=%s,", p->name_.c_str());
        printf("val=%s\n", p->get_str(&x).c_str());
    }
}

static void test_method()
{
    // 创建对象
    cls* c = g_cls_mgr.get_class("XXX");
    obj o = c->create(111, 222, string("xxx"));

    // 调用show，返回值void
    o.call("show");

    // 调用get_state，返回值string
    string ret = o.call("get_state", "ppp", 777).to<string>();
}
static void test_test()
{
    reg();
    test_create();
    test_proper();
    test_get_all_proper();
    test_method();
}



