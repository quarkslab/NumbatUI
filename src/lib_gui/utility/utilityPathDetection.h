#ifndef UTILITY_PATH_DETECTION_H
#define UTILITY_PATH_DETECTION_H

#include "CombinedPathDetector.h"

namespace utility
{
std::shared_ptr<CombinedPathDetector> getCxxVsHeaderPathDetector();
std::shared_ptr<CombinedPathDetector> getCxxHeaderPathDetector();
std::shared_ptr<CombinedPathDetector> getCxxFrameworkPathDetector();
}	 // namespace utility


#endif	  // UTILITY_PATH_DETECTION_H
