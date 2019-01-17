#ifndef VARIANT_HPP
#define VARIANT_HPP

#include <cstdlib>
#include <typeindex>
#include <type_traits>
#include <utility>

/*! 找出最大整数， 类似冒泡 */
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
/*! 找出最大整数， 类似冒泡 */


/*! 获取最大的align */
template <typename ...Args>
struct MaxAligen : std::integral_constant<int, IntegerMax<std::alignment_of<Args>::value...>::value>
{
};
/*! 获取最大的align */


/*! 是否包含类型T */
template <typename T, typename... Types>
struct Contains;

template <typename T, typename TypeHead, typename... Types>
struct Contains <T, TypeHead, Types...>: std::conditional<std::is_same<T, TypeHead>::value, std::true_type, Contains<T, Types...>> ::type
{
};

template <typename T>
struct Contains<T> : std::false_type {};
/*! 是否包含类型T */


/*! 查找类型T是定义的第几个类型 */
template <typename T, typename... Types>
struct Indexof;

template <typename T, typename TypeHead, typename... Types>
struct Indexof<T, TypeHead, Types...>
{
    enum{ value = Indexof<T, Types...>::value + 1};
};

template < typename T, typename... Types>
struct Indexof<T, T, Types...>
{
    enum{ value = 0};
};

template <typename T>
struct Indexof<T>
{
    enum{ value = -1 };
};
/*! 查找类型T是定义的第几个类型 */


/*! 查找第n个定义的是什么类型 */
template<int index, typename... Types>
struct At;

template<int index, typename First, typename... Types>
struct At<index, First, Types...>
{
    using type = At<index - 1, Types...>;
};

template <typename T, typename... Types>
struct At<0, T, Types...>
{
    using type = T;
};
/*! 查找第n个定义的是什么类型 */


template <typename... Types>
class Variant
{
    enum
    {
        data_size = IntegerMax<sizeof(Types)...>::value,
        align_size = MaxAligen<Types...>::value
    };

    using data_t = typename std::aligned_storage<data_size, align_size>::type;

public:
    template<int index>
    using IndexType = typename At<index, Types...>::type;

private:
    std::type_index type_index_;
    data_t data_;
public:
    Variant(void);
    ~Variant();
    Variant(Variant<Types...>&& old) : type_index_(old.type_index)
    {
        move(type_index_, &old.data_, &data_);
    }

    Variant(const Variant<Types...>& old) : type_index_(old.type_index_)
    {
        copy(type_index_, &old.data_, &data_);
    }

    Variant& operator=(const Variant& old)
    {
        type_index_ = old.type_index_;
        copy(type_index_, &old.data_, &data_);
    }

    Variant& operator=(Variant&& old)
    {
        type_index_ = old.type_index_;
        move(type_index_, &old.data_, &data_);
    }

    template<class T, class = typename std::enable_if<Contains<typename std::decay<T>::type, Types...>::value>::type>
    Variant(T&& value) : type_index_(typeid(T))
    {
        destroy(type_index_, &data_);
        typedef typename std::decay<T>::type U;
        new(&data_) U(std::forward<T>(value));
        type_index_ = std::type_index(typeid (U));
    }

    template<typename T>
    bool is() const
    {
        return std::type_index(typeid(T)) == type_index_;
    }

    bool Empty() const
    {
        return std::type_index(typeid(void)) == type_index_;
    }

    std::type_index & type() const
    {
        return type_index_;
    }

    template<typename T>
    typename std::decay<T>::type & get()
    {
        using U = typename std::decay<T>::type;
        if(!is<U>())
        {
//            std::cout << typeid(U).name() << " is not defined. "
//                            << "current type is " << type_index_.name() << std::endl;
            throw std::bad_cast{};
        }

        return *(static_cast<U*>(&data_));
    }

    template<typename T>
    size_t indexof()
    {
        return Indexof<T, Types...>::value;
    }

    bool operator==(const Variant& rhs) const
    {
        return type_index_ == rhs.type_index_;
    }

    bool operator<(const Variant& rhs) const
    {
        return type_index_ < rhs.type_index_;
    }

private:
    void destroy(const std::type_index& index, void* buf)
    {
        [](Types&&...){}((destroy0<Types>(index, buf), 0)...);
    }

    template<typename T>
    void destroy0(const std::type_index& id, void *data)
    {
        if(id == std::type_index(typeid(T)))
            reinterpret_cast<T*>(data)->~T();
    }

    void move(const std::type_index& id, void * old_v, void * new_v)
    {
        [](Types&&...){}((move0<Types>(id, old_v, new_v), 0)...);
    }

    template<typename T>
    void move0(const std::type_index& index, void * old_v, void * new_v)
    {
        if(index == std::type_index(typeid(T)))
            new(new_v) T(std::move(*reinterpret_cast<T*>(old_v)));
    }

    void copy(const std::type_index& id, void* old_v, void* new_v)
    {
        [](Types&&...){}((copy0<Types>(id, old_v, new_v), 0)...);
    }

    template<typename T>
    void copy0(const std::type_index& id, void* old_v, void * new_v)
    {
        if(id == std::type_index(typeid(T)))
            new(new_v) T(* reinterpret_cast<const T*>(old_v));
    }
};

#endif // VARIANT_HPP
