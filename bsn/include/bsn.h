#ifndef _BSN_BSN_H_
#define _BSN_BSN_H_

#include <cu/uncopyable.h>

#include <stdint.h>

namespace json { class Value; }

namespace bsn
{

using byte = uint8_t;

class INode : private cu::Uncopyable
{
public:
	virtual size_t GetBinSize() const = 0;
	virtual void StoreToBin(byte** data, size_t& length) const = 0;
	virtual void StoreToJson(json::Value& val) const = 0;
	virtual ~INode() {}
}; // INode

}

#endif // _BSN_BSN_H_