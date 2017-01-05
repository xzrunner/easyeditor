#include "Particle3DBuilder.h"
#include "PackParticle3D.h"
#include "PackNodeFactory.h"
#include "P3dSprBuilder.h"
#include "PackUI.h"
#include "PackTag.h"

#include <ee/Visitor.h>

#include <easyparticle3d.h>

#include <gum/trans_color.h>
#include <ps_3d.h>

namespace erespacker
{

Particle3DBuilder::Particle3DBuilder()
{
}

Particle3DBuilder::~Particle3DBuilder()
{
	std::map<const eparticle3d::Symbol*, const PackParticle3D*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}
}

void Particle3DBuilder::Traverse(ee::Visitor<IPackNode>& visitor) const
{
	std::map<const eparticle3d::Symbol*, const PackParticle3D*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackParticle3D*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

const IPackNode* Particle3DBuilder::Create(const eparticle3d::Symbol* sym, P3dSprBuilder* spr_builder)
{
	std::map<const eparticle3d::Symbol*, const PackParticle3D*>::iterator 
		itr = m_map_data.find(sym);
	if (itr != m_map_data.end()) {
		return itr->second;
	}

	PackParticle3D* node = new PackParticle3D;
	Load(sym, node);
	m_map_data.insert(std::make_pair(sym, node));
	spr_builder->Create(sym, node);

	PackUI::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());
	PackTag::Instance()->OnKnownPackID(sym->GetFilepath(), node->GetSprID());	

	return node;
}

void Particle3DBuilder::Load(const eparticle3d::Symbol* sym, PackParticle3D* ps)
{
	const p3d_emitter_cfg* cfg = sym->GetEmitterCfg();
	
	ps->blend = cfg->blend;

	ps->static_mode = cfg->static_mode;

	ps->emission_time = cfg->emission_time;
	ps->count = cfg->count;

	ps->life = cfg->life;
	ps->life_var = cfg->life_var;

	ps->hori = cfg->hori;
	ps->hori_var = cfg->hori_var;
	ps->vert = cfg->vert;
	ps->vert_var = cfg->vert_var;

	ps->radial_spd = cfg->radial_spd;
	ps->radial_spd_var = cfg->radial_spd_var;
	ps->tangential_spd = cfg->tangential_spd;
	ps->tangential_spd_var = cfg->tangential_spd_var;
	ps->angular_spd = cfg->angular_spd;
	ps->angular_spd_var = cfg->angular_spd_var;

	ps->dis_region = cfg->dis_region;
	ps->dis_region_var = cfg->dis_region_var;
	ps->dis_spd = cfg->dis_spd;
	ps->dis_spd_var = cfg->dis_spd_var;

	ps->gravity = cfg->gravity;

	ps->linear_acc = cfg->linear_acc;
	ps->linear_acc_var = cfg->linear_acc_var;

	ps->fadeout_time = cfg->fadeout_time;

	ps->ground = cfg->ground;

	ps->start_radius = cfg->start_radius;
	ps->start_height = cfg->start_height;

	ps->orient_to_movement = cfg->orient_to_movement;

	ps->components.reserve(cfg->sym_count);
	for (int i = 0; i < cfg->sym_count; ++i) 
	{
		const p3d_symbol& p_symbol = cfg->syms[i];

		PackParticle3D::Component comp;

		comp.count = p_symbol.count;

		comp.scale_start = p_symbol.scale_start;
		comp.scale_end = p_symbol.scale_end;

		comp.angle = p_symbol.angle;
		comp.angle_var = p_symbol.angle_var;

		comp.mul_col_begin = gum::color2int(s2::Color(p_symbol.mul_col_begin.r, p_symbol.mul_col_begin.g, p_symbol.mul_col_begin.b, p_symbol.mul_col_begin.a), gum::ARGB);
		comp.mul_col_end = gum::color2int(s2::Color(p_symbol.mul_col_end.r, p_symbol.mul_col_end.g, p_symbol.mul_col_end.b, p_symbol.mul_col_end.a), gum::ARGB);
		comp.add_col_begin = gum::color2int(s2::Color(p_symbol.add_col_begin.r, p_symbol.add_col_begin.g, p_symbol.add_col_begin.b, p_symbol.add_col_begin.a), gum::ARGB);
		comp.add_col_end = gum::color2int(s2::Color(p_symbol.add_col_end.r, p_symbol.add_col_end.g, p_symbol.add_col_end.b, p_symbol.add_col_end.a), gum::ARGB);

		ee::Symbol* sym = dynamic_cast<ee::Symbol*>(static_cast<s2::Symbol*>(p_symbol.ud));
		comp.node = PackNodeFactory::Instance()->Create(sym);

		ps->components.push_back(comp);
	}
}

}