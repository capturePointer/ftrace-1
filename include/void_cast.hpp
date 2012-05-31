#ifndef VOID_CAST_HPP
#define VOID_CAST_HPP

namespace ftrace {

template<typename T>
void *void_cast(T ptr)
{
    return *reinterpret_cast<void**>(&ptr);
}

} // namespace ftrace

#endif

