#ifndef DEBUG_H
#define DEBUG_H

#ifdef DEBUG
#include <HardwareSerial.h>
#include <serstream>
#include <iomanip>  // for setprecision()
#include <sstream>
std::ohserialstream serial(Serial);
#define LOG serial
#else
#include <serstream>
#include <ostream>
struct nullstream : std::ostream {
    nullstream() : std::ios(0), std::ostream(0) {}
};
static nullstream logstream;
#define LOG if(0) logstream
#endif

#endif // DEBUG_H

