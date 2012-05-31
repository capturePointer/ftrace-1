#ifndef FSTUB_IMPL_HPP
#define FSTUB_IMPL_HPP

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/algorithm/query/find.hpp>
#include <boost/fusion/include/at_c.hpp>
#include "nullptr.hpp"
#include "ftrace.hpp"
#include "fext.hpp"
#include "void_cast.hpp"
#include "logger.hpp"
#include "fstub_actions.hpp"

namespace ftrace {

template<typename> class fstub_impl;

typedef boost::mpl::vector
<
    fake_call
>
events;

template<typename Ret, typename Base, typename T1>
class fstub_impl<Ret (Base::*)(T1)> : public fext<fstub_impl, Base, Ret (Base::*)(T1), events>
{
    class actions;
    class when_then_comparator;

    typedef Ret (Base::*func_t)(T1);
    typedef fext<ftrace::fstub_impl, Base, func_t, events> fext_t;
    typedef boost::shared_ptr< action<Ret> > action_t;
    typedef boost::fusion::vector<T1> params_t;
    typedef std::vector<params_t> calls_t;
    typedef std::map<params_t, actions, when_then_comparator> when_then_t;

    class when_then_comparator
    {
    public:
         template<typename Left, typename Right>
         bool operator()(const Left& left, const Right& right) const
         {
             return boost::fusion::at_c<0>(left) != boost::fusion::at_c<0>(right);
         }
    };

    class actions : public std::vector<action_t>
    {
    public:
        actions()
            : current_(0)
        { }

        Ret invoke()
        {
            if ((this->begin() + current_ == this->end()))
            {
                throw 0;
            }

            return (this->begin() + current_++)->get()->invoke();
        }

    private:
        std::size_t current_;
    };

    using fext_t::get;
    using fext_t::get_this;

public:
    template<typename T>
    fstub_impl(func_t func, T origin_this)
        : fext_t(func, origin_this)
    { }

    //fstub_impl& when(matcher<T1> t1)
    //fstub_impl& when() //default action

    fstub_impl& when(T1 t1)
    {
        when_then_[params_t(t1)] = actions();
        return *this;
    }
    ///when(3).or.when(3).thenReturn(5).thenReturn(3);
    ///default return 3
    ///sequence
    ///or
    ///what when implementation not found

    fstub_impl& times(unsigned value)
    {
        while(--value)
        {
            when_then_.rbegin()->second.push_back(
                *when_then_.rbegin()->second.rbegin());
        }
        return *this;
    }

    fstub_impl& thenReturn(Ret ret)
    {
        when_then_.rbegin()->second.push_back(
            action_t(new returnable_action<Ret>(ret)));
        return *this;
    }

    template<typename Exception>
    fstub_impl& thenThrow(Exception exception)
    {
        when_then_.rbegin()->second.push_back(
            action_t(new throwable_action<Ret, Exception>(exception)));
        return *this;
    }

    static Ret fake_call(Base* origin_this, T1 t1)
    {
        typename when_then_t::iterator it = get().when_then_.find(params_t(t1));

        try
        {
            if (it != get().when_then_.end())
            {
                get().calls_.push_back(params_t(t1));
                return it->second.invoke();
            }
        }
        catch(...)
        {
            throw;
        }

        union any_pointer
        {
            void* obj_ptr;
            Ret (*func_ptr)(Base*, T1);
        };
        any_pointer p;
        p.obj_ptr = calls_base::get_origin_func();

        //TODO traits -> remove class type
        return ((Ret (*)(Base*, T1))p.func_ptr)(origin_this, t1);
    }

    calls_t& calls()
    {
        return calls_;
    }

private:
    calls_t calls_;
    when_then_t when_then_;
};

} // namespace ftrace

#endif

