#pragma once
#include "class.hpp"
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
    int xx1;
    int xx2;

    std::string xx3;
};

static void reg()
{
    g_cls_mgr.create<XXX>("XXX")
        .add_pro("xx1", &XXX::xx1)
        .add_pro("xx2", &XXX::xx2)
        .add_pro("xx3", &XXX::xx3)
        .add_con<XXX,int,int,string>()
        ;
}

static void test_proper()
{
    cls* c = g_cls_mgr.get_class("XXX");
    obj xx = c->create(110, 119, string("jqka"));

    {
        auto xx1 = xx.get("xx1").to<int>();
        auto xx2 = xx.get("xx2").to<int>();
        auto xx3 = xx.get("xx3").to<string>();

        printf("reflect get1 xx1=%d,xx2=%d,xx3=%s\n", xx1, xx2, xx3.c_str());
    }
    xx.set("xx1", 1000);
    xx.set("xx2", 2000);
    xx.set("xx3", string("2222"));

    {
        auto xx1 = xx.get("xx1").to<int>();
        auto xx2 = xx.get("xx2").to<int>();
        auto xx3 = xx.get("xx3").to<string>();
        printf("reflect get2 xx1=%d,xx2=%d,xx3=%s\n", xx1, xx2, xx3.c_str());
    }
}

static void test_create()
{
    cls* c = g_cls_mgr.get_class("XXX");
    obj o = c->create(110, 119, string("jqka"));
    o.to<XXX>()->show();
}

static void test_get_all_proper()
{
    cls* c = g_cls_mgr.get_class("XXX");
    obj o = c->create(1, 2, string("x"));

    for (auto pro : c->pros_)
    {
        proper* p = pro.second;
        string pro_name = p->name_;
        any val = p->get(&o);
        printf("key=%s,", pro_name.c_str());
        printf("type=%s\n", val.type().raw_name());
    }
}

static void test_test()
{
    reg();
    test_create();
    test_proper();
    test_get_all_proper();
    
}



