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
	void SetID(int id) { m_id = id; }

	void SetFilepath(const std::string& filepath) const {
		m_filepath = filepath;
	}

protected:
	int m_id;

	mutable std::string m_filepath;

}; // IPackNode

class PackNodeCmp
{
public:
	bool operator () (const IPackNode* lhs, const IPackNode* rhs) const {
		return lhs->GetID() < rhs->GetID();
	}
}; // PackNodeCmp

}

#endif // _LIBCOCO_IPACK_NODE_H_