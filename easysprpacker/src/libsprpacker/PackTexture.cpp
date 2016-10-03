#include "PackTexture.h"
#include "binary_io.h"
#include "PackNodeFactory.h"

#include <easyshape.h>
#include <easytexture.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <sprite2/PolygonShape.h>
#include <simp/NodeTexture.h>
#include <simp/simp_types.h>

namespace esprpacker
{

PackTexture::PackTexture(const etexture::Symbol* sym)
{
	const std::vector<s2::PolygonShape*>& polys = sym->GetPolygons();
	for (int i = 0, n = polys.size(); i < n; ++i) 
	{
		s2::PolygonShape* shape = polys[i];
		eshape::Symbol* shape_sym = new eshape::Symbol();
		shape_sym->SetShape(shape);
		const PackNode* node = PackNodeFactory::Instance()->Create(shape_sym);
		m_polys.push_back(node);
	}
}

PackTexture::~PackTexture()
{
	for_each(m_polys.begin(), m_polys.end(), cu::RemoveRefFunctor<const PackNode>());
}

void PackTexture::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"texture\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(m_id));

	std::stringstream ss;
	ss << "polys = {";
	for (int i = 0, n = m_polys.size(); i < n; ++i) {
		ss << m_polys[i]->GetID() << ", ";
	}
	ss << "}";
	gen.line(ss.str());

	gen.detab();
	gen.line("},");
}

int PackTexture::SizeOfUnpackFromBin() const
{
	return simp::NodeTexture::Size() + sizeof(uint32_t) * m_polys.size();
}

int PackTexture::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	// polys
	sz += sizeof(uint16_t);
	sz += sizeof(uint32_t) * m_polys.size();
	return sz;
}

void PackTexture::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = simp::TYPE_TEXTURE;
	pack(type, ptr);

	uint16_t n = m_polys.size();
	pack(n, ptr);

	for (int i = 0; i < n; ++i) {
		uint32_t id = m_polys[i]->GetID();
		pack(id, ptr);
	}
}

}