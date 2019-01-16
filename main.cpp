#include "Any.hpp"
#include "function_traits.hpp"
#include <iostream>
#include <cxxabi.h>
//----------function_traits²âÊÔ´úÂë------------
template <typename T>
void printType()
{
    std::cout << typeid(T).name() << std::endl;
//    int status;
//    std::cout << abi::__cxa_demangle(typeid(T).name(),0,0,&status) << std::endl;
}

float(*castfunc)(float, int);
float free_function(const float & a, int b)
{
    return a/b;
}

struct AA
{
    int f(int a, int b) volatile
    {
        return a+b;
    }

    int operator () (int) const {return 0;}
};

void testFunctionTraits()
{
    std::function<int(int)> f = [](int a){return a;};
    std::cout << "----------std::function<int(int)------------" << std::endl;
    printType<function_traits<std::function<int(int)>>::function_type>();
    printType<function_traits<std::function<int(int)>>::return_type>();
    std::cout << "----------std::function<int(int)> f = [](int a){return a;}------------" << std::endl;
    printType<function_traits<decltype (f)>::function_type >();
    printType<function_traits<decltype (f)>::return_type >();
    std::cout << "----------float free_function(const float & a, int b)------------" << std::endl;
    printType<function_traits<decltype (free_function)>::function_type >();
    printType<function_traits<decltype (free_function)>::return_type >();
    printType<function_traits<decltype (free_function)>::args<0>::type >();
    printType<function_traits<decltype (free_function)>::args<1>::type >();
    std::cout << "----------float(*castfunc)(float, int);------------\n" << std::endl;
    printType<function_traits<decltype (castfunc)>::function_type>();
    std::cout << "---------- AA------------\n" << std::endl;
    printType<function_traits<AA>::function_type>();
    printType<function_traits<decltype (&AA::f)>::function_type>();
}
//----------function_traits²âÊÔ´úÂë------------

int main()
{
    Any n;
    std::cout << n.IsNull() << std::endl;
    std::string s = "hello";
    n = s;
//     n.AnyCast<int>();

    Any n1 = 1;
    std::cout << n1.Is<int>() << std::endl;
    int t = n1.AnyCast<int>();

    std::cout << "t = " << t << std::endl;

    std::cout << "\n\n\n\n----------function_traits------------\n\n\n\n" << std::endl;
    testFunctionTraits();
    std::cout << "\n\n\n\n----------function_traits------------\n\n\n\n" << std::endl;

    std::cout << "typeid.name of int = ";
    printType<int>();
    return 0;
}
