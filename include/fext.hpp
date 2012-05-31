#ifndef FEXT_HPP
#define FEXT_HPP

#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include "calls.hpp"
#include "events.hpp"

namespace ftrace {

template<template<typename> class> class fext_owner;

template
<
    template<typename> class Ext,
    typename Base,
    typename F,
    typename Events
>
class fext
{
    template<typename T>
    class register_events
    {
    public:
        register_events(F func, T origin_this)
            : func(func), origin_this(origin_this)
        { }

        void operator()(const fake_call&) const
        {
            calls<F>::template register_func<fake_call>(origin_this, func, &Ext<F>::fake_call);
        }

    private:
        F func;
        T origin_this;
    };

public:
    template<typename T>
    fext(F func, T origin_this)
    {
        boost::mpl::for_each<Events>(register_events<T>(func, origin_this));
    }

    ~fext()
    {
        //unregister
    }

    //enable if ext_stub
    static Ext<F>& get()
    {
        //return static_cast<fext_owner<Ext>*>(get_this())->template get<F>();
    }

    //enable if ext_stub
    static Base* get_this()
    {
        return static_cast<Base*>(calls_base::calls_map[calls_base::get_origin_func()].ext_this);
    }
};

} // namespace ftrace

#endif

