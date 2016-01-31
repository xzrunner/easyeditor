#include "PackLabel.h"

#include "LabelToLuaString.h"
#include "LabelFromLua.h"

#include "LabelToBin.h"
#include "LabelFromBin.h"

namespace librespacker
{

PackLabel::PackLabel(int id)
	: IPackNode(id)
{
}

void PackLabel::PackToLuaString(ebuilder::CodeGenerator& gen, 
								const ee::TexturePacker& tp,
								float scale) const
{
	LabelToLuaString::Pack(this, gen);
}

void PackLabel::UnpackFromLua(lua_State* L, const std::vector<ee::Image*>& images)
{
	LabelFromLua::Unpack(L, this);
}

int PackLabel::SizeOfPackToBin() const
{
	return LabelToBin::Size(this);
}

void PackLabel::PackToBin(uint8_t** ptr, 
						  const ee::TexturePacker& tp,
						  float scale) const
{
	LabelToBin::Pack(this, ptr);
}

int PackLabel::SizeOfUnpackFromBin() const
{
	return LabelFromBin::Size(this);
}

void PackLabel::UnpackFromBin(uint8_t** ptr, const std::vector<ee::Image*>& images)
{
	LabelFromBin::Unpack(ptr, this);
}

}