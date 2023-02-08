#include <string>
#include <memory.h>
#include <time.h>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    struct tm timeinfo;
    static char buffer [10];
    memset(&timeinfo, 0, sizeof(timeinfo));
    timeinfo.tm_sec = (int) seconds;   
    mktime(&timeinfo);
    sprintf(buffer, "%.2d:%.2d:%.2d\n", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

    return buffer;
}