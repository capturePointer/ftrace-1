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

//TODO friend only with some class in ftrace.cpp
void calls_base::set_origin_func(calls_base::func_t func)
{
    //assert
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


} // namespace ftrace

#endif

/*#include <boost/interprocess/managed_shared_memory.hpp>*/
//#include <boost/interprocess/containers/map.hpp>
//#include <boost/interprocess/allocators/allocator.hpp>
//#include <functional>
//#include <utility>

//int main ()
//{
   //using namespace boost::interprocess;

   ////Remove shared memory on construction and destruction
   //struct shm_remove
   //{
      //shm_remove() { shared_memory_object::remove("MySharedMemory"); }
      //~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
   //} remover;

   ////Shared memory front-end that is able to construct objects
   ////associated with a c-string. Erase previous shared memory with the name
   ////to be used and create the memory segment at the specified address and initialize resources
   //managed_shared_memory segment
      //(create_only
      //,"MySharedMemory" //segment name
      //,65536);          //segment size in bytes

   ////Note that map<Key, MappedType>'s value_type is std::pair<const Key, MappedType>,
   ////so the allocator must allocate that pair.
   //typedef int    KeyType;
   //typedef float  MappedType;
   //typedef std::pair<const int, float> ValueType;

   ////Alias an STL compatible allocator of for the map.
   ////This allocator will allow to place containers
   ////in managed shared memory segments
   //typedef allocator<ValueType, managed_shared_memory::segment_manager>
      //ShmemAllocator;

   ////Alias a map of ints that uses the previous STL-like allocator.
   ////Note that the third parameter argument is the ordering function
   ////of the map, just like with std::map, used to compare the keys.
   //typedef map<KeyType, MappedType, std::less<KeyType>, ShmemAllocator> MyMap;

   ////Initialize the shared memory STL-compatible allocator
   //ShmemAllocator alloc_inst (segment.get_segment_manager());

   ////Construct a shared memory map.
   ////Note that the first parameter is the comparison function,
   ////and the second one the allocator.
   ////This the same signature as std::map's constructor taking an allocator
   //MyMap *mymap =
      //segment.construct<MyMap>("MyMap")      //object name
                                 //(std::less<int>() //first  ctor parameter
                                 //,alloc_inst);     //second ctor parameter

   ////Insert data in the map
   //for(int i = 0; i < 100; ++i){
      //mymap->insert(std::pair<const int, float>(i, (float)i));
   //}
   //return 0;
//}
