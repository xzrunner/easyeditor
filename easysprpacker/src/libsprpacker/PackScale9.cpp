#include "PackScale9.h"
#include "PackNodeFactory.h"
#include "binary_io.h"

#include <easyscale9.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeScale9.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackScale9::PackScale9(const escale9::Symbol* sym)
{
	Init(sym);
}

void PackScale9::PackToLuaString(ebuilder::CodeGenerator& gen,
								 const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"scale9\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));

	lua::assign_with_end(gen, "s9_type", m_type);
	for (int i = 0, n = m_grids.size(); i < n; ++i) 
	{
		const Grid& g = m_grids[i];
		assert(g.node);
		lua::TableAssign ta(gen, "", true);
		lua::connect(gen, 4, 
			lua::assign("id", g.node->GetID()), 
			lua::assign("angle", g.angle),
			lua::assign("xmirror", g.mirror.x),
			lua::assign("ymirror", g.mirror.y));
	}

	gen.detab();
	gen.line("},");
}

int PackScale9::SizeOfUnpackFromBin() const
{
	int n = 9;
	switch (m_type)
	{
	case s2::S9_9GRID:
		n = 9;
		break;
	case s2::S9_3GRID_HORI: case s2::S9_3GRID_VERT:
		n = 3;
		break;
	case s2::S9_6GRID_UPPER:
		n = 6;
		break;
	case s2::S9_9GRID_HOLLOW:
		n = 8;
		break;
	}
	return simp::NodeScale9::Size() + simp::NodeScale9::GridSize() * n;
}

int PackScale9::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint8_t);			// s9 type
	for (int i = 0, n = m_grids.size(); i < n; ++i) {
		const Grid& g = m_grids[i];
		assert (g.node);
		sz += sizeof(uint32_t);		// grid id
		sz += sizeof(uint8_t);		// dir & mirror
	}	
	return sz;
}

void PackScale9::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_SCALE9;
	pack(type, ptr);

	uint8_t s9_type = m_type;
	pack(s9_type, ptr);

	for (int i = 0, n = m_grids.size(); i < n; ++i) 
	{
		const Grid& g = m_grids[i];
		assert(g.node);
		
		uint32_t id = g.node->GetID();
		pack(id, ptr);

		uint8_t dir = ((int)(floor(g.angle * 2 / SM_PI + 0.5f)) + 4) % 4;
		uint8_t mirror = 0;
		if (g.mirror.x) {
			mirror |= 0x1;
		}
		if (g.mirror.y) {
			mirror |= 0x2;
		}
		uint8_t dm = dir << 4 | mirror;
		pack(dm, ptr);
	}
}

void PackScale9::Init(const escale9::Symbol* sym)
{
	const s2::Scale9& s9 = sym->GetScale9();

	m_type = s9.GetType();

	std::vector<s2::Sprite*> sprs;
	s9.GetGrids(sprs);
	m_grids.resize(sprs.size());
	for (int i = 0, n = sprs.size(); i < n; ++i) 
	{
		const s2::Sprite* src = sprs[i];
		Grid& dst = m_grids[i];
		dst.angle = src->GetAngle();
		const sm::vec2& scale = src->GetScale();
		if (scale.x < 0) {
			dst.mirror.x = true;
		}
		if (scale.y < 0) {
			dst.mirror.y = true;
		}
		dst.node = PackNodeFactory::Instance()->Create(
			dynamic_cast<const ee::Sprite*>(src));
	}
}

}