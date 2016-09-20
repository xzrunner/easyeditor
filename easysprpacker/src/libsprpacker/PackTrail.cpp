#include "PackTrail.h"
#include "PackNodeFactory.h"
#include "binary_io.h"
#include "to_int.h"

#include <easytrail.h>
#include <easybuilder.h>
namespace lua = ebuilder::lua;

#include <simp/NodeTrail.h>
#include <simp/simp_types.h>
#include <gum/trans_color.h>

namespace esprpacker
{

PackTrail::PackTrail(const etrail::Symbol* sym)
{
	Init(sym);
}

void PackTrail::PackToLuaString(ebuilder::CodeGenerator& gen, const ee::TexturePacker& tp, float scale) const
{
	gen.line("{");
	gen.tab();

	lua::comments(gen, "file: " + GetFilepath());

	lua::assign_with_end(gen, "type", "\"trail\"");
	lua::assign_with_end(gen, "id", ee::StringHelper::ToString(m_id));
	if (!m_name.empty()) {
		lua::assign_with_end(gen, "export", "\"" + m_name + "\"");
	}

	PackToLuaMT(gen);

	gen.detab();
	gen.line("},");
}

int PackTrail::SizeOfUnpackFromBin() const
{
	int sz = 0;
	sz += simp::NodeTrail::Size();
	int n = 0;
	if (m_mode == T2D_MODE_IMAGE) {
		n = m_comp_images.size();
	} else {
		assert(m_mode == T2D_MODE_SHAPE);
		n = m_comp_shapes.size();
	}
	sz += simp::NodeTrail::ComponentSize() * n;
	return sz;
}

int PackTrail::SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint8_t);			// type
	sz += sizeof(uint8_t);			// mode
	// components
	sz += sizeof(uint16_t);
	if (m_mode == T2D_MODE_IMAGE) {
		for (int i = 0, n = m_comp_images.size(); i < n; ++i) {
			sz += m_comp_images[i].SizeOfPackToBin();
		}
	} else {
		assert(m_mode == T2D_MODE_SHAPE);
		for (int i = 0, n = m_comp_shapes.size(); i < n; ++i) {
			sz += m_comp_shapes[i].SizeOfPackToBin();
		}
	}
	sz += sizeof(uint16_t);			// count
	sz += sizeof(uint16_t);			// life_begin
	sz += sizeof(uint16_t);			// life_offset
	sz += sizeof(uint16_t);			// fadeout_time
	return sz;
}

void PackTrail::PackToBin(uint8_t** ptr, const ee::TexturePacker& tp, float scale) const
{
	uint32_t id = m_id;
	pack(id, ptr);

	uint8_t type = simp::TYPE_TRAIL;
	pack(type, ptr);

	uint8_t mode = m_mode;
	pack(mode, ptr);

	// components
	if (m_mode == T2D_MODE_IMAGE) 
	{
		uint16_t sz = m_comp_images.size();
		pack(sz, ptr);
		for (int i = 0; i < sz; ++i) {
			m_comp_images[i].PackToBin(ptr);
		}
	} 
	else 
	{
		assert(m_mode == T2D_MODE_SHAPE);
		uint16_t sz = m_comp_shapes.size();
		pack(sz, ptr);
		for (int i = 0; i < sz; ++i) {
			m_comp_shapes[i].PackToBin(ptr);
		}
	}

	uint16_t count = m_count;
	pack(count, ptr);

	uint16_t life_begin = float100x2int(m_life_begin);
	pack(life_begin, ptr);
	uint16_t life_offset = float100x2int(m_life_offset);
	pack(life_offset, ptr);

	uint16_t fadeout_time = float100x2int(m_fadeout_time);
	pack(fadeout_time, ptr);
}

void PackTrail::Init(const etrail::Symbol* sym)
{
	const t2d_emitter_cfg* cfg = sym->GetEmitterCfg();

	m_mode			= cfg->mode_type;

	m_count			= cfg->count;

	m_life_begin	= cfg->life_begin;
	m_life_offset	= cfg->life_offset;

	m_fadeout_time	= cfg->fadeout_time;

	if (m_mode == T2D_MODE_IMAGE) {
		m_comp_images.reserve(cfg->sym_count);
		for (int i = 0; i < cfg->sym_count; ++i) {
			m_comp_images.push_back(CompImage(cfg->syms[i]));
		}
	} else {
		assert(m_mode == T2D_MODE_SHAPE);
		m_comp_shapes.reserve(cfg->sym_count);
		for (int i = 0; i < cfg->sym_count; ++i) {
			m_comp_shapes.push_back(CompShape(cfg->syms[i]));
		}
	}
}

void PackTrail::PackToLuaMT(ebuilder::CodeGenerator& gen) const
{
	lua::connect(gen, 2, 
		lua::assign("mode", m_mode),
		lua::assign("count", m_count));

	lua::connect(gen, 3, 
		lua::assign("life_begin", m_life_begin),
		lua::assign("life_offset", m_life_offset),
		lua::assign("fadeout_time", m_fadeout_time));

	lua::TableAssign ta(gen, "component", true);
	if (m_mode == T2D_MODE_IMAGE) {
		for (int i = 0, n = m_comp_images.size(); i < n; ++i) {
			m_comp_images[i].PackToLuaString(gen);
		}	
	} else {
		assert(m_mode == T2D_MODE_SHAPE);
		for (int i = 0, n = m_comp_shapes.size(); i < n; ++i) {
			m_comp_shapes[i].PackToLuaString(gen);
		}
	}
}

/************************************************************************/
/* class PackTrail::CompImage                                           */
/************************************************************************/

PackTrail::CompImage::
CompImage(const t2d_symbol& sym)
{
	ee::Symbol* ee_sym	= dynamic_cast<ee::Symbol*>(static_cast<s2::Symbol*>(sym.mode.A.ud));
	m_node				= PackNodeFactory::Instance()->Create(ee_sym);

	m_scale_begin		= sym.mode.A.scale_begin;
	m_scale_end			= sym.mode.A.scale_end;

	m_mul_col_begin		= gum::color2int(sym.col_begin.rgba, gum::RGBA);
	m_mul_col_end		= gum::color2int(sym.col_end.rgba, gum::RGBA);
	m_add_col_begin		= gum::color2int(sym.mode.A.add_col_begin.rgba, gum::RGBA);
	m_add_col_end		= gum::color2int(sym.mode.A.add_col_end.rgba, gum::RGBA);
}

PackTrail::CompImage::
~CompImage()
{
	m_node->RemoveReference();
}

void PackTrail::CompImage::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::TableAssign ta(gen, "", true);

	lua::connect(gen, 1, 
		lua::assign("id", m_node->GetID()));

	lua::connect(gen, 2, 
		lua::assign("scale_begin", m_scale_begin),
		lua::assign("scale_end", m_scale_end));

	lua::connect(gen, 4, 
		lua::assign("mul_col_begin", m_mul_col_begin), 
		lua::assign("mul_col_end", m_mul_col_end),
		lua::assign("add_col_begin", m_add_col_begin), 
		lua::assign("add_col_end", m_add_col_end));
}

int PackTrail::CompImage::
SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint32_t);			// id
	sz += sizeof(uint16_t) * 2;		// scale
	sz += sizeof(uint32_t) * 4;		// color
	return sz;
}

void PackTrail::CompImage::
PackToBin(uint8_t** ptr) const
{
	uint32_t id = m_node->GetID();
	pack(id, ptr);

	uint16_t scale_begin = float100x2int(m_scale_begin);
	pack(scale_begin, ptr);
	uint16_t scale_end = float100x2int(m_scale_end);
	pack(scale_end, ptr);

	uint32_t mul_col_begin = m_mul_col_begin;
	pack(mul_col_begin, ptr);
	uint32_t mul_col_end = m_mul_col_end;
	pack(mul_col_end, ptr);
	uint32_t add_col_begin = m_add_col_begin;
	pack(add_col_begin, ptr);
	uint32_t add_col_end = m_add_col_end;
	pack(add_col_end, ptr);
}

/************************************************************************/
/* class PackTrail::CompShape                                           */
/************************************************************************/

PackTrail::CompShape::
CompShape(const t2d_symbol& sym)
{
	m_linewidth		= sym.mode.B.size;
	m_acuity		= sym.mode.B.acuity;

	m_col_begin		= gum::color2int(sym.col_begin.rgba, gum::RGBA);
	m_col_end		= gum::color2int(sym.col_end.rgba, gum::RGBA);
}

void PackTrail::CompShape::
PackToLuaString(ebuilder::CodeGenerator& gen) const
{
	lua::TableAssign ta(gen, "", true);

	lua::connect(gen, 2, 
		lua::assign("linewidth", m_linewidth),
		lua::assign("acuity", m_acuity));

	lua::connect(gen, 2, 
		lua::assign("col_begin", m_col_begin),
		lua::assign("col_end", m_col_end));
}

int PackTrail::CompShape::
SizeOfPackToBin() const
{
	int sz = 0;
	sz += sizeof(uint16_t);			// linewidth
	sz += sizeof(uint16_t);			// acuity
	sz += sizeof(uint32_t) * 2;		// color
	return sz;
}

void PackTrail::CompShape::
PackToBin(uint8_t** ptr) const
{
	uint16_t linewidth = float100x2int(m_linewidth);
	pack(linewidth, ptr);

	uint16_t acuity = float100x2int(m_acuity);
	pack(acuity, ptr);

	uint32_t col_begin = m_col_begin;
	pack(col_begin, ptr);
	uint32_t col_end = m_col_end;
	pack(col_end, ptr);
}

}