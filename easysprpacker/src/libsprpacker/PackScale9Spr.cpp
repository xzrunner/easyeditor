#include "PackScale9Spr.h"

#include <easyscale9.h>

#include <SM_Vector.h>

namespace esprpacker
{

PackScale9Spr::PackScale9Spr(const escale9::Sprite* spr)
{
	Init(spr);
}

void PackScale9Spr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, 
									float scale) const
{

}

int PackScale9Spr::SizeOfUnpackFromBin() const
{

}

int PackScale9Spr::SizeOfPackToBin() const
{

}

void PackScale9Spr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, 
							  float scale) const
{

}

void PackScale9Spr::Init(const escale9::Sprite* spr)
{
	sm::vec2 sz = spr->GetScale9().GetSize();
	m_width = sz.x;
	m_height = sz.y;
}

}