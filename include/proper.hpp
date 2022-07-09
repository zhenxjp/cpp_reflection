#pragma once

#include <iostream>
#include <sstream>
#include <map>
#include <any>
#include <typeindex>
using namespace std;

class proper
{
public:
    virtual any get(void* c) = 0;
    virtual void set(void* c, any val) = 0;
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

    virtual void set(void* c, any val) {
        C& real = *((C*)c);
        real.*pro_ = std::any_cast<F>(val);
    }
public:
    F C::* pro_;
};