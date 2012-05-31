#ifndef FEXT_OWNER_HPP
#define FEXT_OWNER_HPP

namespace ftrace {

template<template<typename> class Ext>
class fext_owner
{
public:
    fext_owner()
        : is_initalized_(false)
    { }

    template<typename T, typename Type> void init(const T& func, const Type& type)
    {
        LOG(trace) << is_initalized_;

        if (!is_initalized_)
        {
            ext_ = Ext<T>(func, type);
        }
        is_initalized_ = true;
    }

    template<typename T> Ext<T>& get()
    {
        return boost::any_cast<Ext<T>&>(ext_);
    }

private:
    boost::any ext_;
    bool is_initalized_;
};

} // namespace ftrace

#endif

