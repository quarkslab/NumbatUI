#include "HoverText.h"
std::map<Id, std::string> HoverText::text;

HoverText HoverText::parse(std::string& full_hover_text)
{
	std::stringstream ss(full_hover_text);
	std::string option;
	std::string metadata;
	getline(ss, option, ' ');
	getline(ss, metadata);
	return {option, metadata};
}
