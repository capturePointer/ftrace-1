#include <iostream>
#include <stdexcept>
#include <cassert>
#include <boost/mpl/assert.hpp>
#include <tut/tut.hpp>
#include <tut/tut_macros.hpp>
#include "nullptr.hpp"
#include "logger.hpp"

#include "extensions/fstub/fstub.hpp"

namespace tut {

struct fstub_
{
    fstub_() { }
    virtual ~fstub_() { }
};

typedef test_group<fstub_> factory;
typedef factory::object object;

} // namespace tut

namespace {
tut::factory tf("fstub");
} // namespace anonymous

namespace tut {

class K
{
public:
    virtual int f(int a, const char* b)
    {
        LOG(debug) << "a: " << a << "b: " << b;
        return 2;
    }

    template<int T> int bla(const char*)
    {
        LOG(debug) << "T: " << T;
        return T;
    }
};

template<> template<> void object::test<1>()
{
        .then(Return(32))
        .then(Call(&B:aa))
        .then(Throw<E>(&B:aa))



    //given
    ftrace::fstub<K> stub;
    //stub(&K::bla<3>).when().thenReturn(7);//default behavior
    //stub(&K::bla<3>).when().thenCall(&K::bla<3>);//default behavior original
    //stub(&K::bla<3>).when().thenReturn();
    stub(&K::bla<3>).when("s").thenReturn(7).times(2).thenReturn(4).thenThrow(std::out_of_range("ex"));
    //stub(&K::bla<3>).when("s").thenReturn(7).times(3);
    //stub(&K::bla<3>).thenReturn(7);//default value

    K& l_k = stub.getInstance();
    K k;

    //expected_call(&K::bla<3>).when(_, _)

    //when & then
    ensure_equals(l_k.bla<3>("s"), 7);
    ensure_equals(l_k.bla<3>("s"), 7);
    ensure_equals(l_k.bla<3>("s"), 4);
    ensure_THROW(ensure_equals(l_k.bla<3>("s"), 4), std::out_of_range);
    //ensure_equals(l_k.bla<3>("f"), 3);
/*    ensure_equals(k.bla<3>("s"), 3);*/

    //ensure_equals(stub(&K::bla<3>).calls().size(), std::size_t(3));
    /*ensure_equals(boost::fusion::at_c<0>(stub(&K::bla<3>).calls()[0]), "s");*/

    stub(&K::bla<3>).calls().clear();

    //Calls type with additional methods
}

} // namespace tut

