#ifndef _LIBCOCO_IPACK_NODE_H_
#define _LIBCOCO_IPACK_NODE_H_

#include <string>

namespace ebuilder { class CodeGenerator; }

namespace libcoco
{

class TexturePacker;

class IPackNode
{
public:
	IPackNode() {
		static int ID = 0;
		m_id = ID++;
	}
	virtual ~IPackNode() {}

	virtual void ToString(ebuilder::CodeGenerator& gen,
		const TexturePacker& tp) const = 0;

	int GetID() const { return m_id; }

protected:
	int m_id;

}; // IPackNode

}

#endif // _LIBCOCO_IPACK_NODE_H_