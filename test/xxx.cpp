
#include <iostream>
#include <map>
#include "cpp_now.hpp"
#include "test_class.hpp"

using namespace std;


void test1()
{
    XXX xxx{ 111,222,"xxx" };

    int XXX::* xx1_pro = &XXX::xx1;// 关键的

    printf("xx1=%d\n", xxx.*xx1_pro);
    xxx.*xx1_pro = 888;
    printf("xx1=%d\n", xxx.*xx1_pro);
}



int main(int argc, char** argv)
{
    test_cpp_now();
    test_test();

    return 0;
}
