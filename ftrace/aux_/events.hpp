#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <boost/mpl/vector.hpp>

namespace ftrace {

template<bool override_call = false>
struct event_base
{
    static const bool value = override_call;
};

struct fake_call : event_base<true> { };
struct fenter_call : event_base<> { };
struct fexit_call : event_base<> { };
struct fenter_info : event_base<> { };
struct fexit_info : event_base<> { };

typedef boost::mpl::vector
<
    fake_call,          ///called on enter function, parameters forwarded
    fenter_call,        ///called on enter function, parameters forwarded
    fexit_call,         ///called on exit function, parameters forwarded
    fenter_info,        ///called on enter function, parameters ignored
    fexit_info          ///called on exit function, parameters ignored
>
events_types;

} // namespace ftrace

#endif

//template<typename T1> Ret call(T1 t1)
//template<typename T1> void call(T1 t1)
//void call(void* what, void* from);

