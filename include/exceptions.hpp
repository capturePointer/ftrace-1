#include <exception>

namespace mockitopp
{
   struct partial_implementation_exception : public ::std::exception
   {
      const char *what() const throw()
         { return "function has partial implementation!"; }
   };

   struct missing_implementation_exception : public ::std::exception
   {
      const char *what() const throw()
         { return "function has no implementation!"; }
   };

   //undefined behavior

} // namespace mockitopp

