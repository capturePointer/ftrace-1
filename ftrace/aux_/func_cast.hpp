#ifndef FUNC_CAST_HPP
#define FUNC_CAST_HPP

namespace ftrace {

template<typename T>
T func_cast(void* ptr)
{
    //mpl assert is function
    union fcast
    {
        void* obj_ptr;
        T func_ptr;
    };

    fcast cast;
    cast.obj_ptr = ptr;

    return cast.func_ptr;
}

} // namespace ftrace

#endif

