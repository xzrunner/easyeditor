#include "PackClipbox.h"

#include "ClipboxToLuaString.h"
#include "ClipboxFromLua.h"

#include "ClipboxToBin.h"
#include "ClipboxFromBin.h"

namespace librespacker
{

PackClipbox::PackClipbox(int id)
	: IPackNode(id)
{
}

void PackClipbox::PackToLuaString(ebuilder::CodeGenerator& gen,
								  const ee::TexturePacker& tp,
								  float scale) const
{
	ClipboxToLuaString::Pack(this, gen);
}

void PackClipbox::UnpackFromLua(lua_State* L, const std::vector<ee::Image*>& images)
{
	ClipboxFromLua::Unpack(L, this);
}

int PackClipbox::SizeOfPackToBin() const
{
	return ClipboxToBin::Size();
}

void PackClipbox::PackToBin(uint8_t** ptr, 
							const ee::TexturePacker& tp,
							float scale) const
{
	ClipboxToBin::Pack(this, ptr);
}

int PackClipbox::SizeOfUnpackFromBin() const
{
	return ClipboxFromBin::Size();
}

void PackClipbox::UnpackFromBin(uint8_t** ptr, const std::vector<ee::Image*>& images)
{
	ClipboxFromBin::Unpack(ptr, this);
}

}