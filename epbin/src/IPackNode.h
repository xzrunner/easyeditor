#ifndef _EPBIN_I_PACK_NODE_H_
#define _EPBIN_I_PACK_NODE_H_

#include <stdint.h>
#include <fstream>

namespace epbin
{

class IPackNode
{
public:
	IPackNode(uint8_t type, uint16_t id) 
		: m_type(type), m_id(id), m_maxid(0) {}

	virtual size_t Size() const = 0;
	virtual void Store(std::ofstream& fout) const = 0;

	uint8_t GetType() const { return m_type; }

	uint16_t GetID() const { return m_id; }
	uint16_t GetMaxID() const { return m_maxid; }

private:
	uint8_t m_type;
	uint16_t m_id;

protected:
	uint16_t m_maxid;

}; // IPackNode

}

#endif // _EPBIN_I_PACK_NODE_H_