#ifndef _FUNCTION_TRAITS_H
#define _FUNCTION_TRAITS_H
#include <iostream>
#include <type_traits>

template<typename T>
struct function_traits;

template<typename Ret, typename... Args>
struct function_traits<Ret(Args...)>
{
public:
    enum { argNum = sizeof...(Args) };
    typedef Ret function_type(Args...);
    typedef Ret return_type;
    typedef Ret(*functionPtr)(Args...);
    typedef std::function<function_type> stdFunctionType;

    template<size_t N>
    struct arg_type
    {
        static_assert(N < argNum, "Error! argument index greater than argument size");
        typedef typename std::tuple_element<N, std::tuple<Args...>>::type type;
    };
};

//the function pointer
template <typename Ret, typename... Args>
struct function_traits<Ret(*)(Args...)> : public function_traits<Ret(Args...)>{};

//the std function
template<typename Ret, typename... Args>
struct function_traits<std::function<Ret(Args...)>>:public function_traits<Ret(Args...)>{};

//the callable 
template<typename CallableObject>
struct function_traits: public function_traits<decltype(&CallableObject::operator())> {};

//member function
#define MEMBER_FUNCTION_TRAITS(...) \
template<typename Ret, typename ClassType, typename... Args> \
struct function_traits<Ret(ClassType::*)(Args...) __VA_ARGS__> : public function_traits<Ret(Args...)>{};\


MEMBER_FUNCTION_TRAITS()
MEMBER_FUNCTION_TRAITS(const)

#endif
