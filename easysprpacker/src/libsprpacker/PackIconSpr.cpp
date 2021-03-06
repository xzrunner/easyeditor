#include "PackIconSpr.h"
#include "PackNodeFactory.h"
#include "binary_io.h"

#include <easyicon.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeIconSpr.h>
#include <simp/simp_types.h>
#include <bs/FixedPointNum.h>

namespace esprpacker
{

PackIconSpr::PackIconSpr(const std::shared_ptr<eicon::Sprite>& spr)
{
	m_sym = PackNodeFactory::Instance()->Create(
		std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol()));

	m_process = spr->GetProcess();
}

PackIconSpr::~PackIconSpr()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void PackIconSpr::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"icon_spr\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(GetID()));

	lua::connect(gen, 2, 
		lua::assign("sym_id", m_sym->GetID()), 
		lua::assign("process", m_process));

	gen.detab();
	gen.line("},");
}

int PackIconSpr::SizeOfUnpackFromBin() const
{
	return simp::NodeIconSpr::Size();
}

int PackIconSpr::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint32_t);			// sym id
	sz += sizeof(uint16_t);			// process
	return sz;
}

void PackIconSpr::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp) const
{
	uint32_t id = GetID();
	pack(id, ptr);

	uint8_t type = simp::TYPE_ICON_SPR;
	pack(type, ptr);

	uint32_t sym = m_sym->GetID();
	pack(sym, ptr);

	uint16_t process = bs::float2int(m_process, 1024);
	pack(process, ptr);
}

bool PackIconSpr::Equal(const eicon::Sprite* spr) const
{
	return m_process == spr->GetProcess();
}

}