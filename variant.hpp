#ifndef VARIANT_HPP
#define VARIANT_HPP

#include <cstdlib>
#include <typeindex>
#include <type_traits>

/*! �ҳ���������� ����ð������ */
template <size_t arg, size_t... rest>
struct IntegerMax;

template <size_t arg>
struct IntegerMax<arg>: std::integral_constant<size_t, arg>{};

template <size_t arg1, size_t arg2, size_t... args>
struct IntegerMax <arg1, arg2, args...> :
        std::integral_constant<size_t, arg1 >= arg2 ?
        IntegerMax<arg1, args...>::value : IntegerMax<arg2, args...>::value>
{
};
/*! �ҳ���������� ����ð������ */


/*! ��ȡ����align */
template <typename ...Args>
struct MaxAligen : std::integral_constant<int, IntegerMax<std::alignment_of<Args>::value...>::value>
{
};
/*! ��ȡ����align */


/*! �Ƿ��������T */
template <typename T, typename... Types>
struct Contains;

template <typename T, typename TypeHead, typename... Types>
struct Contains <T, TypeHead, Types...>: std::conditional<std::is_same<T, TypeHead>::value, std::true_type, Contains<T, Types...>> ::type
{
};

template <typename T>
struct Contains<T> : std::false_type {};
/*! �Ƿ��������T */


template<typename ...Args>
struct Variant
{

};

#endif // VARIANT_HPP
