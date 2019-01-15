#include "Any.hpp"
#include "function_traits.hpp"

namespace FunctionTraitsTest
{
    template <typename T>
    void printType(T type)
    {
        std::cout << type.name() << std::endl;
    }
}

int main()
{
    Any n;
    std::cout << n.IsNull() << std::endl;
    std::string s = "hello";
    n = s;
    // n.AnyCast<int>();

    Any n1 = 1;
    std::cout << n1.Is<int>() << std::endl;
    int t = n1.AnyCast<int>();

    std::cout << "t = " << t << std::endl;

    return 0;
}
