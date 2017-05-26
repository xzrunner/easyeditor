#include "TrailBuilder.h"
#include "PackTrail.h"
#include "PackNodeFactory.h"
#include "PackUI.h"
#include "PackTag.h"

#include <ee/Visitor.h>

#include <easytrail.h>

#include <sprite2/TrailEmitterCfg.h>
#include <gum/trans_color.h>
#include <mt_2d.h>

namespace erespacker
{

TrailBuilder::TrailBuilder(ExportNameSet& export_set)
	: m_export_set(export_set)
{	
}

TrailBuilder::~TrailBuilder()
{
	std::map<const etrail::Symbol*, const PackTrail*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}
}

void TrailBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
	std::map<const etrail::Symbol*, const PackTrail*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackTrail*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

const IPackNode* TrailBuilder::Create(const etrail::Symbol* sym)
{
	std::map<const etrail::Symbol*, const PackTrail*>::iterator 
		itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		return itr->second;
	}

	PackTrail* node = new PackTrail;
	Load(sym, node);
	m_map_data.insert(std::make_pair(sym, node));
	m_export_set.LoadExport(sym, node);

	PackUI::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());
	PackTag::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());	

	return node;
}

void TrailBuilder::Load(const etrail::Symbol* sym, PackTrail* trail)
{
	const t2d_emitter_cfg* cfg = sym->GetEmitterCfg()->GetImpl();

	trail->mode = cfg->mode_type;

	trail->count = cfg->count;

	trail->life_begin = cfg->life_begin;
	trail->life_offset = cfg->life_offset;

	trail->fadeout_time = cfg->fadeout_time;

	if (trail->mode == T2D_MODE_IMAGE) {
		trail->comp_images.reserve(cfg->sym_count);
		for (int i = 0; i < cfg->sym_count; ++i) {
			const t2d_symbol& t_sym = cfg->syms[i];

			PackTrail::CompImage comp;

			ee::Symbol* sym = dynamic_cast<ee::Symbol*>(static_cast<s2::Symbol*>(t_sym.mode.A.ud));
			comp.node = PackNodeFactory::Instance()->Create(sym);

			comp.scale_begin = t_sym.mode.A.scale_begin;
			comp.scale_end = t_sym.mode.A.scale_end;

			comp.mul_col_begin = gum::color2int(s2::Color(t_sym.col_begin.r, t_sym.col_begin.g, t_sym.col_begin.b, t_sym.col_begin.a), s2::ABGR);
			comp.mul_col_end = gum::color2int(s2::Color(t_sym.col_end.r, t_sym.col_end.g, t_sym.col_end.b, t_sym.col_end.a), s2::ABGR);
			comp.add_col_begin = gum::color2int(s2::Color(t_sym.mode.A.add_col_begin.r, t_sym.mode.A.add_col_begin.g, t_sym.mode.A.add_col_begin.b, t_sym.mode.A.add_col_begin.a), s2::ABGR);
			comp.add_col_end = gum::color2int(s2::Color(t_sym.mode.A.add_col_end.r, t_sym.mode.A.add_col_end.g, t_sym.mode.A.add_col_end.b, t_sym.mode.A.add_col_end.a), s2::ABGR);

			trail->comp_images.push_back(comp);
		}
	} else {
		assert(trail->mode == T2D_MODE_SHAPE);
		trail->comp_shapes.reserve(cfg->sym_count);
		for (int i = 0; i < cfg->sym_count; ++i) {
			const t2d_symbol& t_sym = cfg->syms[i];

			PackTrail::CompShape comp;

			comp.linewidth = t_sym.mode.B.size;
			comp.acuity = t_sym.mode.B.acuity;

			comp.col_begin = gum::color2int(s2::Color(t_sym.col_begin.r, t_sym.col_begin.g, t_sym.col_begin.b, t_sym.col_begin.a), s2::ABGR);
			comp.col_end = gum::color2int(s2::Color(t_sym.col_end.r, t_sym.col_end.g, t_sym.col_end.b, t_sym.col_end.a), s2::ABGR);

			trail->comp_shapes.push_back(comp);
		}
	}
}

}