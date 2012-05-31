#ifndef FTARCE_HPP
#define FTARCE_HPP

#include <cstring>
#include <ucontext.h>
#include "stack_frame.hpp"
#include "arch/arch.hpp"
#include "calls.hpp"
#include "logger.hpp"

extern "C" {
    void __cyg_profile_func_enter(void *, void *) __attribute__((no_instrument_function));
    void __cyg_profile_func_exit(void *, void *) __attribute__((no_instrument_function));
}


namespace {
enum command {
    InsDisabled,
    InsEnabled,
    CallStub,
    RestoreIP
};
} // namespace anonymous

void __cyg_profile_func_enter(void *this_fn, void *)
{
    using namespace ftrace;

    static command next_command = InsEnabled;
    static ucontext_t context;
    //static ucontext_t context_tmp;
    static greg_t ip = 0; //should be vector
    static calls_base::func_t call_ptr = 0;
    if (next_command == InsDisabled)return;

    getcontext(&context);

    switch (next_command)
    {
        case InsDisabled:
        {
            return;
        }

        case CallStub:
        {
            asm("call *%0" : : "r"(*&call_ptr));
            next_command = RestoreIP;
        }
        return;

        case RestoreIP:
        {
            next_command = InsEnabled;
            ftrace::stack_frame* frame = reinterpret_cast<stack_frame*>(context.uc_mcontext.gregs[Spec<arch>::REG_BP]);
            *reinterpret_cast<greg_t*>(&frame->next[1]) = ip;
        }
        return;

        default:
        {
            //std::memcpy(&context, &context_tmp, sizeof(ucontext_t));
            //context = context_tmp;
            break;
        }
    }

    next_command = InsDisabled;
    if (calls_base::exists_func(this_fn))
    {
        calls_base::set_origin_func(this_fn);
        call_ptr = calls_base::get_func_call(this_fn);

        stack_frame* frame = reinterpret_cast<stack_frame*>(context.uc_mcontext.gregs[Spec<arch>::REG_BP]);
        Spec<arch>::restore_regs(context, frame->next);
        context.uc_mcontext.gregs[Spec<arch>::REG_SP] = reinterpret_cast<greg_t>(2 + frame->next);
        context.uc_mcontext.gregs[Spec<arch>::REG_BP] = reinterpret_cast<greg_t>(frame->next->next);
        ip = *reinterpret_cast<greg_t*>(&frame->next[1]);

        next_command = CallStub;
        setcontext(&context);
    }
    next_command = InsEnabled;
}

void __cyg_profile_func_exit(void *, void *)
{
}

#endif

