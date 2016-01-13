#ifndef _LIBRESPACKER_IPACK_NODE_H_
#define _LIBRESPACKER_IPACK_NODE_H_

#include <drag2d.h>

namespace ebuilder { class CodeGenerator; }

namespace librespacker
{

class IPackNode : public d2d::Object
{
public:
	IPackNode();
	IPackNode(int spr_id);
	virtual ~IPackNode() {}

	// lua
	virtual void PackToLuaString(ebuilder::CodeGenerator& gen,
		const d2d::TexturePacker& tp, float scale) const = 0;
	virtual void UnpackFromLua(lua_State* L,
		const std::vector<d2d::Image*>& images) = 0;

	// bin
	virtual int SizeOfPackToBin() const = 0;
	virtual void PackToBin(uint8_t** ptr,
		const d2d::TexturePacker& tp, float scale) const = 0;
	virtual int SizeOfUnpackFromBin() const = 0;
	virtual void UnpackFromBin(uint8_t** ptr, 
		const std::vector<d2d::Image*>& images) = 0;

	int GetPkgID() const { return m_pkg_id; }
	int GetSprID() const { return m_spr_id; }

	void SetFilepath(const std::string& filepath) const {
		m_filepath = filepath;
	}
	const std::string& GetFilepath() const {
		return m_filepath;
	}

public:
	std::string export_name;

protected:
	int m_pkg_id;
	int m_spr_id;

	mutable std::string m_filepath;

}; // IPackNode

class PackNodeCmp
{
public:
	bool operator () (const IPackNode* lhs, const IPackNode* rhs) const {
		return lhs->GetSprID() < rhs->GetSprID();
	}
}; // PackNodeCmp

}

#endif // _LIBRESPACKER_IPACK_NODE_H_