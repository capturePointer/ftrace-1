#ifndef NULLPTR_HPP
#define NULLPTR_HPP

#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_member_pointer.hpp>
#include <boost/mpl/or.hpp>
#include <boost/utility/enable_if.hpp>

namespace nullptr_detail {

class nullptr_type
{
public:
    template<typename T>
    operator T *() const { return 0; }

    template<typename C, typename T>
    operator T C:: *() const { return 0; }

private:
    void operator&() const;
};

inline bool operator==(nullptr_type, nullptr_type)
{
    return true;
}

inline bool operator!=(nullptr_type, nullptr_type)
{
    return false;
}

template<typename T>
inline typename boost::enable_if<
    boost::mpl::or_<
        boost::is_pointer<T>,
        boost::is_member_pointer<T>
    >,
    bool
>::type operator==(T left, nullptr_type)
{
    return left == 0;
}

template<typename T>
inline typename boost::enable_if<
    boost::mpl::or_<
        boost::is_pointer<T>,
        boost::is_member_pointer<T>
    >,
    bool
>::type operator==(nullptr_type left, T right)
{
    return right == 0;
}

template<typename T>
inline typename boost::enable_if<
    boost::mpl::or_<
        boost::is_pointer<T>,
        boost::is_member_pointer<T>
    >,
    bool
>::type operator!=(T left, nullptr_type right)
{
    return !(left == right);
}

template<typename T>
inline typename boost::enable_if<
    boost::mpl::or_<
        boost::is_pointer<T>,
        boost::is_member_pointer<T>
    >,
    bool
>::type operator!=(nullptr_type left, T right)
{
    return !(left == right);
}

} // namespace nullptr_detail

typedef nullptr_detail::nullptr_type nullptr_t;

const nullptr_t nullptr = nullptr_t();

#endif

