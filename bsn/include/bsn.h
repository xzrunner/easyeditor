#ifndef _BSN_BSN_H_
#define _BSN_BSN_H_

#include <cu/uncopyable.h>

#include <stdint.h>

namespace bsn
{

using byte = uint8_t;

class ISerializable
{
public:
	virtual size_t GetByteArraySize() const = 0;
	virtual void LoadFromByteArray(const byte* data) = 0;
	virtual void StoreToByteArray(byte** data, size_t& length) const = 0;
	virtual ~ISerializable() {}
}; // ISerializable

class INode : private cu::Uncopyable, public ISerializable
{
public:
	virtual ~INode() {}
}; // INode

}

#endif // _BSN_BSN_H_