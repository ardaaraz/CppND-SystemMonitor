#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function

string Format::ElapsedTime(long seconds) {
    std::string s = "00:00:00";
    long times = 0;
    if(seconds > 0)
    {
        // Hours
        times  = seconds / 3600;
        s      = TimetoString(times) + ":";
        // Minutes
        times  = (seconds / 60) % 60;
        s     += TimetoString(times) + ":";
        // Seconds
        times  = seconds % 60;
        s     += TimetoString(times);
    }   
    return s;
}

string Format::TimetoString(long times) {
    if(times < 10) { return "0" + std::to_string(times); }
    else { return std::to_string(times); }
}
