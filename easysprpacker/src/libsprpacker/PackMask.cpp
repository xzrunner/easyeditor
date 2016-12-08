#include "PackMask.h"
#include "PackNodeFactory.h"
#include "binary_io.h"

#include <easymask.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeMask.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackMask::PackMask(const emask::Symbol* sym)
{
	m_base = PackNodeFactory::Instance()->Create(dynamic_cast<const ee::Sprite*>(sym->GetBase()));
	m_mask = PackNodeFactory::Instance()->Create(dynamic_cast<const ee::Sprite*>(sym->GetMask()));
}

PackMask::~PackMask()
{
	m_base->RemoveReference();
	m_mask->RemoveReference();
}

void PackMask::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"mask\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));
	if (!GetName().empty()) {
		lua::assign_with_end(gen, "export", "\"" + GetName() + "\"");
	}

	lua::connect(gen, 2, 
		lua::assign("base_id", m_base->GetID()),
		lua::assign("mask_id", m_mask->GetID()));

	gen.detab();
	gen.line("},");
}

int PackMask::SizeOfUnpackFromBin() const
{
	return simp::NodeMask::Size();
}

int PackMask::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);						// id
	sz += sizeof(uint8_t);						// type
	sz += sizeof(uint32_t);						// base id
	sz += sizeof(uint32_t);						// mask id
	return sz;
}

void PackMask::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_MASK;
	pack(type, ptr);

	assert(m_base);
	uint32_t base_id = m_base->GetID();
	pack(base_id, ptr);

	assert(m_mask);
	uint32_t mask_id = m_mask->GetID();
	pack(mask_id, ptr);
}

}