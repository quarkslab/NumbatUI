#ifndef STORAGE_NODE_TYPE_H
#define STORAGE_NODE_TYPE_H

#include <string>

#include "types.h"

struct StorageNodeType
{
	StorageNodeType(): id(0), type(""), kind("") {}
	StorageNodeType(Id id, std::string type, std::string kind): id(id), type(type), kind(kind) {}

	bool operator<(const StorageNodeType& other) const
	{
		return id < other.id;
	}

	Id id;
	std::string type;
	std::string kind;
};

#endif	  // STORAGE_NODE_TYPE_H