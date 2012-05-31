#ifndef REMOVE_MEMBER_FUNCTION_HPP
#define REMOVE_MEMBER_FUNCTION_HPP

namespace ftrace {

template<typename T>
struct remove_member_function
{
    typedef T type;
};

template<typename Ret, typename C>
struct remove_member_function<Ret (C::*)()>
{
    typedef Ret (*type)(C*);
};

template<typename Ret, typename C, typename T1>
struct remove_member_function<Ret (C::*)(T1)>
{
    typedef Ret (*type)(C*, T1);
};

} // namespace ftrace

#endif

