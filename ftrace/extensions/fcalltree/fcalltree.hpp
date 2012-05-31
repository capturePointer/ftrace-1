simple reflection or dwarf info about parameters
simple example with int and some class

#include <dlfcn.h>
#include <iostream>
#include <cxxabi.h>
#include <vector>
#include <boost/algorithm/string.hpp>

    namespace Dupa
{
    template<int, bool>
    class Q{
    public:
        static void dupa(int a, int b)
        {
        }
};
}

int main(int argc, char **argv)
{

    Dl_info l;
    dladdr((void*)&Dupa::Q<3, true>::dupa, &l);
    std::cout << l.dli_sname <<std::endl;

    std::string value(__cxxabiv1::__cxa_demangle(
                l.dli_sname,
                0,
                0,
                0
            ));

    std::cout << value;

}

g++ -o dladdr dladdr.cpp -ldl -rdynamic

