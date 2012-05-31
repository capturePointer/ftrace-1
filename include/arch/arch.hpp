#ifndef ARCH_HPP
#define ARCH_HPP

namespace ftrace {
template<typename> class Spec;
} // namespace ftrace

#if defined(__x86_64__)
# include "arch/x86_64/x86_64.hpp"
#elif defined(__i386__)
# include "arch/x86/x86.hpp"
#else
# error architecture not supported
#endif

#endif

