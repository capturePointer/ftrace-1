#ifndef X86_HPP
#define X86_HPP

#include <ucontext.h>
#include "stack_frame.hpp"

namespace ftrace {

struct x86 { };
typedef x86 arch;

template<> class Spec<x86>
{
public:
    enum { REG_SP = REG_ESP };
    enum { REG_BP = REG_EBP };

    static void restore_regs(ucontext_t& context, const stack_frame* frame) __attribute__((no_instrument_function));
    static void set_regs(ucontext_t& context, greg_t sp, greg_t bp) __attribute__((no_instrument_function));
};

void Spec<x86>::restore_regs(ucontext_t&, const stack_frame*)
{ }

void Spec<x86>::set_regs(ucontext_t& context, greg_t sp, greg_t bp)
{
    context.uc_mcontext.gregs[Spec<arch>::REG_SP] = sp;
    context.uc_mcontext.gregs[Spec<arch>::REG_BP] = bp;
}

} // namespace ftrace

#endif

