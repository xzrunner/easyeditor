#ifndef _LIBCOCO_IPACK_NODE_H_
#define _LIBCOCO_IPACK_NODE_H_

#include <string>
#include <vector>

struct lua_State;

namespace ebuilder { class CodeGenerator; }
namespace d2d { class Image; }

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

	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const TexturePacker& tp) const = 0;

	virtual void UnpackFromLua(lua_State* L,
		const std::vector<d2d::Image*>& images) = 0;

	int GetID() const { return m_id; }

protected:
	int m_id;

}; // IPackNode

}

#endif // _LIBCOCO_IPACK_NODE_H_