#if !defined(HOVER_TEXT_H)
#define HOVER_TEXT_H

#include "types.h"

#include <string>
#include <sstream>
#include <map>

struct HoverText{
    static std::map<Id,std::string> text;
    
    static HoverText parse(std::string& full_hover_text);

    std::string option;
    std::string metadata;
};

#endif // HOVER_TEXT_H
