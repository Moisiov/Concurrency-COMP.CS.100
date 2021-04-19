// COMP.CS.310 Concurrency project spring 2021
// https://plus.tuni.fi/comp.cs.310/spring-2021/project3/desc/
// Do make changes to this file, and create more source
// files in this directory if needed.
// Make sure everything is added to course-gitlab before submitting

#include "concur2021.hh"
#include <iostream>
#include <string>
#include <unordered_map>
#include <thread>
#include <vector>
#include <future>
#include <mutex>
#include <numeric>
#include "threadpool.h"

const unsigned SAMPLES = 1000;
std::mutex m;

// data structure to count how many time we have seen
// a location (binary id in lib)
typedef uint64_t Counter;
std::unordered_map< concur2021::locationID_t, Counter > locCounters;

void countLocation( concur2021::locationID_t id )
{
    // save for later use that we have seen the location
    // (in 1st read C++ will initialize value to zero)
    locCounters[ id ] = locCounters[id] + 1;
}

void printCounters() {
    std::cout << "-------------------------------------------------------------" << std::endl;
    Counter tot = std::accumulate(locCounters.begin(), locCounters.end(), 0,
                                   [](const Counter previous, const auto& el)
                                        {return previous + el.second; });

    std::cout << "Total count: " << tot << std::endl;
    // print number of time we have seen a location
    for( auto& id : locCounters ) {
        auto name = concur2021::locationName( id.first );
        std::cout << name << " seen " << id.second << " times." << std::endl;
    }
}


void WheresAhto() {
    m.lock();
    auto location = concur2021::detect(); // API tells us the location (binary id)
    m.unlock();

    // print out the name and location URL right away:
    auto locname  = concur2021::locationName( location );
    std::cout << "Found at: " << locname << std::endl;
    std::cout << concur2021::locationURL( location ) << std::endl;

    // record the location seen
    countLocation( location );
}

/**
 * @brief main is the starting point of the program
 * @return exit status
 */
int main() {

    // ThreadPool handles task execution in threads
    ThreadPool* pool = new ThreadPool();

    // Add tasks to task queue
    for(unsigned i = 0; i < SAMPLES; i++)
    {
        pool->enqueue(WheresAhto);
    }

    // Destroy ThreadPool object (wait all tasks to finish)
    delete pool;

    // Print result
    printCounters();

    return EXIT_SUCCESS;

}
