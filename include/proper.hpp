#pragma once

#include "value.hpp"
#include <string>
#include <sstream>
using namespace std;

class proper
{
public:
    // c:����ָ��
    virtual value get(void* c) = 0;

    // c:����ָ��
    virtual void set(void* c, value val) = 0;

    // c:����ָ��
    virtual string get_str(void* c) = 0;

    string name_;
};

template <class C, typename F>
class proper_impl : public proper
{
public:
    virtual string get_str(void* c)
    {
        stringstream ss;
        ss << get(c).to<F>();
        return ss.str();
    }
    virtual value get(void* c)
    {
        C& real = *((C*)c);
        value ret = real.*pro_;
        return ret;
    }

    virtual void set(void* c, value val) {
        C& real = *((C*)c);
        real.*pro_ = val.to<F>();
    }
public:
    F C::* pro_;
};