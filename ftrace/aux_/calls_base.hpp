#ifndef CALLS_BASE_HPP
#define CALLS_BASE_HPP

#include <cassert>
#include <map>
#include <vector>
#include <boost/variant.hpp>
#include "logger.hpp"
#include "nullptr.hpp"
#include "events.hpp"

namespace ftrace {

class calls_base
{
public:
    typedef void* func_t;
    typedef std::vector<func_t> calls_t;

private:
    struct calls_list
    {
        boost::make_variant_over<events_types>::type event_type;

        func_t fake_call;
        void* ext_this;//TODO fixme
        calls_t to_be_called;
        bool is_locked;
    };

public:
    typedef std::map<func_t, calls_list> calls_map_t;

    static func_t get_func_call(func_t func);
    static void set_origin_func(func_t func);
    static func_t get_origin_func();
    static bool exists_func(func_t func);

    static calls_map_t calls_map;

private:
    static func_t func;
};

calls_base::calls_map_t calls_base::calls_map;
calls_base::func_t calls_base::func = nullptr;

calls_base::func_t calls_base::get_func_call(calls_base::func_t func)
{
    LOG(trace) << std::hex << "func: " << func;

    assert(calls_map.find(func) != calls_map.end());
    return calls_map[func].fake_call;
}

void calls_base::set_origin_func(calls_base::func_t func)
{
    calls_base::func = func;
}

calls_base::func_t calls_base::get_origin_func()
{
    //assert
    return calls_base::func;
}

bool calls_base::exists_func(calls_base::func_t func)
{
    //assert
    calls_map_t::iterator it = calls_map.find(func);
    bool value = it->second.is_locked;
    it->second.is_locked = false;

    return !value && it != calls_map.end();
}

} // namespace ftrace

#endif

