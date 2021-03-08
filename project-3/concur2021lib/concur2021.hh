// COMP.CS.310 Concurrency spring 2021, mandatory project 3
// https://plus.tuni.fi/comp.cs.310/spring-2021/
// DO NOT change anything in this file
//
#ifndef CONCUR2021_HH
#define CONCUR2021_HH

#include <cstdint>
#include <string>

namespace concur2021 {

    // Binary ID for one location
    typedef uint_least64_t locationID_t;

    // Returns one location "detected".
    // THREADS: NOT threadsafe
    locationID_t detect( void );

    // Returns name of a location
    // THREADS: threadsafe
    // PRE: Parameter "id" must be returned by the detect() -function
    std::string locationName( locationID_t id );

    // Returns URL of a location
    // THREADS: threadsafe
    // PRE: Parameter "id" must be returned by the detect() -function
    std::string locationURL( locationID_t id );

}

#endif // CONCUR2021_HH

