// COMP.CS.310 Concurrency spring 2021, mandatory project 3
// https://plus.tuni.fi/comp.cs.310/spring-2021/
// Feel free to read the code, but do not make any coding decision based on this.
// Only the API (concur2021.hh) documentation can be used when creating your
// concurrent version.
// DO NOT change anything in this file
//

#include "concur2021.hh"
#include <stdexcept>
#include <array>
#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <map>
#include <tuple>
#include <random>
#include <thread>

namespace concur2021 {

namespace { // library privates

const unsigned int NUM_ITEMS = 35;
std::random_device rd;
std::mt19937 gen(5);
std::uniform_int_distribution<> dis(0,NUM_ITEMS-1);

typedef std::tuple< concur2021::locationID_t, std::string, std::string, std::string > globalLocation_t;
std::array< globalLocation_t, NUM_ITEMS > blobs =
{
        std::make_tuple( 0x8581e4b19524e21, "Trivandrum, India", "8°29′N  76°57′E", "https://www.openstreetmap.org/#map=12/8.483/76.950"),
        std::make_tuple( 0x6ac8c6fb9678941, "The Hague, Netherlands", "52°05′N  4°19′E", "https://www.openstreetmap.org/#map=12/52.083/4.317"),
        std::make_tuple( 0xb41358271990954, "Banda Aceh, Indonesia", "5°33′N  95°19′E", "https://www.openstreetmap.org/#map=12/5.550/95.317"),
        std::make_tuple( 0x3e53e01c28fdb87, "Mecca, Saudi Arabia", "21°25′N  39°49′E", "https://www.openstreetmap.org/#map=12/21.417/39.817"),
        std::make_tuple( 0x5cfd9a2000f3d18, "São Paulo, Brazil", "23°33′S  46°38′W", "https://www.openstreetmap.org/#map=12/-23.550/-46.633"),
        std::make_tuple( 0xe542dca06126390, "Perth, Australia", "31°57′S  115°52′E", "https://www.openstreetmap.org/#map=12/-31.950/115.867"),
        std::make_tuple( 0x4b7551296e264fd, "Bilbao, Spain", "43°15′N  2°55′W", "https://www.openstreetmap.org/#map=12/43.250/-2.917"),
        std::make_tuple( 0xbe4b558eda28f8f, "Guwahati, India", "26°11′N  91°44′E", "https://www.openstreetmap.org/#map=12/26.183/91.733"),
        std::make_tuple( 0x785da6dac6ed4ff, "Regina, Canada", "50°27′N  104°36′W", "https://www.openstreetmap.org/#map=12/50.450/-104.600"),
        std::make_tuple( 0x3bbb64d610a416c, "Mexicali, Mexico", "32°40′N  115°28′W", "https://www.openstreetmap.org/#map=12/32.667/-115.467"),
        std::make_tuple( 0xbfeeaaf8486e228, "Canberra, Australia", "35°18′S  149°07′E", "https://www.openstreetmap.org/#map=12/-35.300/149.117"),
        std::make_tuple( 0xbf76055b09d7fd9, "Comodoro Rivadavia, Argentina", "45°52′S  67°29′W", "https://www.openstreetmap.org/#map=12/-45.867/-67.483"),
        std::make_tuple( 0x8038e15fa0cabd8, "Tolyatti, Russia", "53°31′N  49°25′E", "https://www.openstreetmap.org/#map=12/53.517/49.417"),
        std::make_tuple( 0x3de94b5e4055f99, "Miri, Malaysia", "4°24′N  114°00′E", "https://www.openstreetmap.org/#map=12/4.400/114.000"),
        std::make_tuple( 0x23294f96814d64d, "Seoul, South Korea", "37°34′N  126°59′E", "https://www.openstreetmap.org/#map=12/37.567/126.983"),
        std::make_tuple( 0x90725c015191daf, "Bandar Seri Begawan, Brunei", "4°53′N  114°57′E", "https://www.openstreetmap.org/#map=12/4.883/114.950"),
        std::make_tuple( 0x7985c13cffd20f5, "Nara, Japan", "34°41′N  135°48′E", "https://www.openstreetmap.org/#map=12/34.683/135.800"),
        std::make_tuple( 0xbe1054c898d700c, "Islamabad, Pakistan", "33°43′N  73°04′E", "https://www.openstreetmap.org/#map=12/33.717/73.067"),
        std::make_tuple( 0xc794796287cd3a4, "Bucharest, Romania", "44°26′N  26°06′E", "https://www.openstreetmap.org/#map=12/44.433/26.100"),
        std::make_tuple( 0x5612b196b0e7d85, "Helena, United States", "46°36′N  112°02′W", "https://www.openstreetmap.org/#map=12/46.600/-112.033"),
        std::make_tuple( 0xafb700f3783778e, "Juiz de Fora, Brazil", "21°46′S  43°21′W", "https://www.openstreetmap.org/#map=12/-21.767/-43.350"),
        std::make_tuple( 0x105460dc986a7c4, "Norwich, United Kingdom", "52°38′N  1°18′E", "https://www.openstreetmap.org/#map=12/52.633/1.300"),
        std::make_tuple( 0x9a6a4ea306ddc1e, "Rosario, Argentina", "32°57′S  60°40′W", "https://www.openstreetmap.org/#map=12/-32.950/-60.667"),
        std::make_tuple( 0xeef6ffc4740875f, "Limon, Costa Rica", "9°59′N  83°02′W", "https://www.openstreetmap.org/#map=12/9.983/-83.033"),
        std::make_tuple( 0x2723e1004f1f9e0, "Natal, Brazil", "5°47′S  35°12′W", "https://www.openstreetmap.org/#map=12/-5.783/-35.200"),
        std::make_tuple( 0x243e921ce4eb9c3, "Hanoi, Vietnam", "21°02′N  105°51′E", "https://www.openstreetmap.org/#map=12/21.033/105.850"),
        std::make_tuple( 0x1834254ba7ee362, "Edmonton, Canada", "53°32′N  113°30′W", "https://www.openstreetmap.org/#map=12/53.533/-113.500"),
        std::make_tuple( 0x9051c5ac0e422d4, "Antananarivo, Madagascar", "18°56′S  47°31′E", "https://www.openstreetmap.org/#map=12/-18.933/47.517"),
        std::make_tuple( 0xe9dd910bf0b1269, "Chengdu,China", "30°40′N  104°04′E", "https://www.openstreetmap.org/#map=12/30.667/104.067"),
        std::make_tuple( 0x7db8133548afbfd, "Hiroshima, Japan", "34°23′N  132°27′E", "https://www.openstreetmap.org/#map=12/34.383/132.450"),
        std::make_tuple( 0x6caf40084a96d1f, "Stanley, United Kingdom", "51°42′S  57°52′W", "https://www.openstreetmap.org/#map=12/-51.700/-57.867"),
        std::make_tuple( 0x84692a9146b54a6, "Athens, Greece", "37°58′N  23°43′E", "https://www.openstreetmap.org/#map=12/37.967/23.717"),
        std::make_tuple( 0x6d6006cad6cc5a1, "Salta, Argentina", "24°47′S  65°25′W", "https://www.openstreetmap.org/#map=12/-24.783/-65.417"),
        std::make_tuple( 0xccbbcfb51dcd8cc, "Pudasjärvi, Finland", "65°22′N  27°00′E", "https://www.openstreetmap.org/#map=12/65.367/27.000"),
        std::make_tuple( 0xbac8c2077308c9c, "Yaren District, Nauru", "0°33′S  166°55′E", "https://www.openstreetmap.org/#map=12/-0.550/166.917")
};

#if 0 // not used 2011
static std::map< locationID_t, std::tuple< globalLocation_t >> mapping;
int initMapping()
{
    for( auto item : blobs ) {
        mapping[ std::get<0>(item) ] = item;
    }
    return 42;
}
int __attribute__((unused)) justtriggerinit = initMapping();
#endif

void my_assert( bool cond, const char* WHAT, const char* FILE, int LINE ) {
    if(!cond) {
        std::cerr << FILE << ":" << LINE << ":"
                  << WHAT << ":" << "Assertion failed" << std::endl;
        exit(1);
    }
}
#define MY_ASSERT( cond ) my_assert(cond, #cond, __FILE__, __LINE__)

std::atomic<int> DetectorCounter(0);

} // unnamed namespace

locationID_t detect( void )
{
    DetectorCounter++;
    std::this_thread::sleep_for( std::chrono::microseconds( dis(gen)*100 ) );
    MY_ASSERT( DetectorCounter.load() == 1 );

    auto idx = dis(gen);
    auto ret = std::get<0>( blobs.at(idx) );

    DetectorCounter--;
    std::this_thread::sleep_for( std::chrono::microseconds( dis(gen)*20 ) );
    MY_ASSERT( DetectorCounter.load() == 0 );

    return ret;
}

std::string locationName( locationID_t id )
{
    for( auto& item : blobs ) {
        if( std::get<0>(item) == id )
            return std::get<1>(item);
    }
    throw std::runtime_error( "Illegal locationID" );
}
std::string locationURL( locationID_t id )
{
    unsigned int count = 1;
    for( auto& item : blobs ) {
        std::this_thread::sleep_for( std::chrono::microseconds( dis(gen)*count++*2 ) );
        if( std::get<0>(item) == id )
            return std::get<3>(item);
    }
    throw std::runtime_error( "Illegal locationID" );

}

} // concur2021 namespace
