
#include <iostream>
#include <map>
#include "cpp_now.hpp"
#include "test_class.hpp"

using namespace std;


void test1()
{
    using XXX_property = int XXX::*;
    XXX_property pro_xx1 = &XXX::xx1;
    XXX_property pro_xx2 = &XXX::xx2;

    XXX xxx{ 111,222,"xxx" };
    int xx1 = xxx.*pro_xx1;
    int xx2 = xxx.*pro_xx2;

    xxx.*pro_xx1 = 888;
    xxx.*pro_xx2 = 999;

    xx1 = xxx.*pro_xx1;
    xx2 = xxx.*pro_xx2;
}



int main(int argc, char** argv)
{
    test_cpp_now();
    test_test();

    return 0;
}
