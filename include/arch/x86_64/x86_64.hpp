#ifndef X86_64_HPP
#define X86_64_HPP

#include <ucontext.h>
#include "stack_frame.hpp"

namespace ftrace {

struct x86_64 { };
typedef x86_64 arch;

template<> class Spec<x86_64>
{
public:
    enum { REG_SP = REG_RSP };
    enum { REG_BP = REG_RBP };

    static void restore_regs(ucontext_t& context, const stack_frame* frame) __attribute__((no_instrument_function));
    static void set_regs(ucontext_t& context, greg_t sp, greg_t bp) __attribute__((no_instrument_function));
};

void Spec<x86_64>::restore_regs(ucontext_t& context, const stack_frame* frame)
{
    context.uc_mcontext.gregs[REG_RDI] = *((int*)(frame) - 6);
    context.uc_mcontext.gregs[REG_RSI] = *((int*)(frame) - 8);
}

void Spec<x86_64>::set_regs(ucontext_t& context, greg_t sp, greg_t bp)
{
    context.uc_mcontext.gregs[Spec<arch>::REG_SP] = sp;
    context.uc_mcontext.gregs[Spec<arch>::REG_BP] = bp;
}

} // namespace ftrace

#endif

