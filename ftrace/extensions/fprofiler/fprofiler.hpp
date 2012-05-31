own malloc
own free
timer - bottleneck
how many enter to function, how many exits
graphiz with graph - as in google profiler

#include <ctime>
#include <iostream>

int main(){
    std::clock_t start( std::clock() );
    for( int i = 0 ; i < 1e9; ++i );
    std::cout
        << std::difftime( std::clock(), start )/((double)CLOCKS_PER_SEC)
        << 's' << std::endl;
}

fprofiler<file> profiler("fprofiler.txt");


fprofiler<memory, plan_file> profiler("a.txt");
fprofiler<cpu, graphiz> profiler("dupa.txt");
