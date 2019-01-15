
#include <iostream>
template<typename Arg>
int print(Arg arg)
{
	std::cout << arg << std::endl;
	return 0;
}

template<typename... Args>
void testPrint(Args... args)
{
    std::initializer_list<int>{(print(args))...};
	//auto f = [&]{std::cout << args << std::endl;return 0;};
	//std::initializer_list<int>{(f())...};
    //��Ҫ�°汾��g++��֧��lambda���ʽ�Ĳ�����չ��
//	std::initializer_list<int>{([&]{std::cout << args << std::endl;return 0;}())...};
}

//int main()
//{
//	testPrint(1,2,4,45,67,2,"hello");
//	return 0;
//}
