#if !defined(HOVER_TEXT_H)
#define HOVER_TEXT_H

#include "types.h"

#include <string>
#include <sstream>
#include <map>

struct HoverText{
    static std::map<Id,std::string> text;
};

#endif // HOVER_TEXT_H
