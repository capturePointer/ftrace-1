#ifndef FSTUB_HPP
#define FSTUB_HPP

#include <boost/shared_ptr.hpp>
#include <boost/any.hpp>
#include "fstub_impl.hpp"
#include "fext_owner.hpp"

namespace ftrace {

template<typename Base, template<typename> class Stub = fstub_impl>
class fstub : public fext_owner<Stub>
{
    typedef fext_owner<Stub> fext_owner_t;

public:
    template<typename T> Stub<T>& operator()(T func)
    {
        init(func, this);
        return fext_owner_t::template get<T>();
    }

    template<typename T> const Stub<T>& verify(T)
    {
        return fext_owner_t::template get<T>();
    }

    Base& getInstance()
    {
        return reinterpret_cast<Base&>(*this);
    }
};

} // namespace ftrace

#endif

