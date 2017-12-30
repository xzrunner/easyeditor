#pragma once

#include <stdint.h>

namespace Json { class Value; }

namespace bsn
{

class NodeSym
{
public:
	virtual ~NodeSym() {}

	//
	// serialization
	//
	virtual void StoreToBin(uint8_t** data, size_t& length) const = 0;
	virtual void StoreToJson(Json::Value& val) const = 0;

}; // NodeSym

}