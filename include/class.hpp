#pragma once
#include "proper.hpp"
#include "args.hpp"
#include "construct.hpp"
#include <map>
using namespace std;

class cls;
class obj
{
public:
    template<class C>
    C* to() {
        return (C*)real_;
    }

    value get(const char* name);
    bool set(const char* name, value val);

    void* real_ = nullptr;
    cls* cls_ = nullptr;
};

class cls
{
public:
    template <class C, typename F>
    cls& add_pro(const char* name,F C::* pro)
    {
        auto pro_ptr = new proper_impl<C, F>();
        pro_ptr->pro_ = pro;
        pro_ptr->name_ = name;
        pros_[name] = pro_ptr;
        return *this;
    }

    template <class C, typename... A>
    cls& add_con()
    {
        con_ = new construct_impl<C, A...>();
        return *this;
    }
public:
    template <typename... A>
    obj create(A... arg)
    {
        void* real = con_->create(arg...);
        obj ret{ real,this };
        return ret;
    }
public:
    proper* get_pro(const char* name)
    {
        auto it = pros_.find(name);
        if (pros_.end() == it)
            return nullptr;
        else
            return it->second;
    }

public:
    map<string, proper*>   pros_;
    construct* con_ = nullptr;
};

class cls_mgr
{
public:
    template<class C>
    cls& create(const char* name) {
        auto* c = new cls();
        cls_map_[name] = c;
        return *c;
    }

    cls* get_class(const char* name)
    {
        if (0 == cls_map_.count(name))
            return nullptr;
        return cls_map_[name];
    }
public:
    map<string, cls*>   cls_map_;
};
static cls_mgr g_cls_mgr;
////////////////////////////////////////////////////



value obj::get(const char* name)
{
    proper* pro = cls_->get_pro(name);
    if (nullptr != pro)
        return pro->get(real_);
    else
        return value();
}

bool obj::set(const char* name, value val)
{
    proper* pro = cls_->get_pro(name);
    if (nullptr == pro)
        return false;

    pro->set(real_, val);
    return true;
}