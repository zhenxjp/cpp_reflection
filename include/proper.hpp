#pragma once

#include "value.hpp"
#include <string>
using namespace std;

class proper
{
public:
    virtual any get(void* c) = 0;
    virtual void set(void* c, value val) = 0;
    string name_;
};

template <class C, typename F>
class proper_impl : public proper
{
public:
    virtual any get(void* c)
    {
        C& real = *((C*)c);
        any ret = real.*pro_;
        return ret;
    }

    virtual void set(void* c, value val) {
        C& real = *((C*)c);
        real.*pro_ = val.to<F>();
    }
public:
    F C::* pro_;
};