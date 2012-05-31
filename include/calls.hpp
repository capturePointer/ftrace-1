#ifndef CALLS_HPP
#define CALLS_HPP

#include <boost/foreach.hpp>
#include <boost/variant.hpp>
#include "nullptr.hpp"
#include "calls_base.hpp"
#include "logger.hpp"

namespace ftrace {

template<typename> class calls;

template<typename Ret, typename Base, typename T1>
class calls<Ret (Base::*)(T1)>
{
    typedef Ret (Base::*func_t)(T1);

    class is_fake_call : public boost::static_visitor<bool>
    {
    public:
        template<typename T>
        bool operator()(const T&) const
        {
            return T::value;
        }
    };

    static Ret call(const fake_call&, calls_base::calls_map_t::iterator it, calls_base::func_t f, Base* origin_this, T1 t1)
    {
        union any_pointer
        {
            void* obj_ptr;
            Ret (*func_ptr)(Base*);
        };
        any_pointer p;

        //TODO 0 -special value means that call this function any way

        if (origin_this == it->second.ext_this)
        {
            p.obj_ptr = f;
        }
        else
        {
            it->second.is_locked = true;
            p.obj_ptr = calls_base::get_origin_func();
        }

        return ((Ret (*)(Base*, T1))p.func_ptr)(origin_this, t1);
    }

    static Ret fake_call_(Base* origin_this, T1 t1)
    {
        calls_base::func_t call_func = calls_base::get_origin_func();
        assert(call_func != nullptr);
        calls_base::calls_map_t::iterator it = calls_base::calls_map.find(call_func);

        LOG(trace) << "method call: " << std::hex << call_func << ", origin_this: " << origin_this;

        if (it != calls_base::calls_map.end())
        {
            LOG(info) << "calls found for: " << std::hex << call_func;

            //BOOST_FOREACH(calls_base::func_t f, it->second.to_be_called)
            //{
                //if (!boost::apply_visitor(is_fake_call(), it->second.event_type))
                //{
                //}
                //else
                //{
                    //return call(it->second.event_type, it, f, origin_this, t1);
                //}
            //}
            //TODO begin

            BOOST_FOREACH(calls_base::func_t f, it->second.to_be_called)
            {
                union any_pointer
                {
                    void* obj_ptr;
                    Ret (*func_ptr)(Base*);
                };
                any_pointer p;

                //TODO 0 -special value means that call this function any way

                if (origin_this == it->second.ext_this)
                {
                    p.obj_ptr = f;
                }
                else
                {
                    it->second.is_locked = true;
                    p.obj_ptr = calls_base::get_origin_func();
                }

                return ((Ret (*)(Base*, T1))p.func_ptr)(origin_this, t1);
            }
            //TODO end
        }

        //call original
        //return ((Ret (*)(Base*, T1))p.func_ptr)(origin_this, t1);
        throw 0;
    }

public:
    template<typename Event, typename T>
    static void register_func(T origin_this, func_t origin_func, Ret (fake_func)(Base*, T1))
    {
        //TODO only one ext_stub function might be registered
        //TODO return stub must be at then end

        LOG(trace) << std::hex << "origin_func: " << origin_func;

        assert(origin_func != nullptr);
        assert(fake_func != nullptr);

        //TODO check if exists
        calls_base::calls_list calls;
        calls.fake_call = void_cast(&calls::fake_call_);
        calls.ext_this = void_cast(origin_this);
        calls.to_be_called.push_back(void_cast(fake_func));
        calls.event_type = Event();
        calls.is_locked = false;


        //TODO callback for
        //example callback:
            //1.getFunc() == original_func
            //2.dladdr().params.size()


        calls_base::calls_map[void_cast(origin_func)] = calls;
    }
};

} // namespace ftrace

#endif

