#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);  // TODO: See src/format.cpp

std::string TimetoString(long times); // Convert time value to desired string format
};                                    // namespace Format

#endif