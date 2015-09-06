#include "PackLabel.h"

#include "LabelToLuaString.h"
#include "LabelFromLua.h"

#include "LabelToBin.h"

namespace libcoco
{

PackLabel::PackLabel(int id)
	: IPackNode(id)
{
}

void PackLabel::PackToLuaString(ebuilder::CodeGenerator& gen, const d2d::TexturePacker& tp) const
{
	LabelToLuaString::Pack(this, gen);
}

void PackLabel::UnpackFromLua(lua_State* L, const std::vector<d2d::Image*>& images)
{
	LabelFromLua::Unpack(L, this);
}

int PackLabel::SizeOfPackToBin() const
{
	return LabelToBin::Size();
}

void PackLabel::PackToBin(uint8_t** ptr, const d2d::TexturePacker& tp) const
{
	LabelToBin::Pack(this, ptr);
}

}