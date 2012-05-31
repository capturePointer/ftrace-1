#ifndef BACKTRACE_HPP
#define BACKTRACE_HPP

#include <boost/function.hpp>
#include <iostream>
#include <execinfo.h>

namespace ftrace {

namespace detail {

class cerr_callback
{
public:
    void operator()(const char* trace)
    {
        std::cerr << trace << std::endl;
    }
};

} // namespace detail

template<typename T = detail::cerr_callback>
class backtrace
{
public:
    //TODO get stack depth
    static void show(unsigned call_stack_size = 20, T callback = detail::cerr_callback())
    {
        void** buffer = new void*[call_stack_size];
        char** bt;

        const std::size_t count = ::backtrace(buffer, call_stack_size);

        if (!count)
        {
            return;
        }

        bt = backtrace_symbols(buffer, count);

        if (!bt)
        {
            return;
        }

        for (std::size_t i = 0; i < count; ++i)
        {
            callback(bt[i]);
        }

        delete[] buffer;
        delete[] bt;
    }
};

} // namespace ftrace

#endif

