#include "Particle3DBuilder.h"
#include "PackParticle3D.h"
#include "PackNodeFactory.h"
#include "P3dSprBuilder.h"

#include <ee/Visitor.h>

#include <easyparticle3d.h>

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

void Particle3DBuilder::Traverse(ee::Visitor& visitor) const
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

const IPackNode* Particle3DBuilder::Create(const eparticle3d::Symbol* symbol, P3dSprBuilder* spr_builder)
{
	std::map<const eparticle3d::Symbol*, const PackParticle3D*>::iterator 
		itr = m_map_data.find(symbol);
	if (itr != m_map_data.end()) {
		return itr->second;
	}

	PackParticle3D* node = new PackParticle3D;
	Load(symbol, node);
	m_map_data.insert(std::make_pair(symbol, node));
	spr_builder->Create(symbol, node);
	return node;
}

void Particle3DBuilder::Load(const eparticle3d::Symbol* symbol, PackParticle3D* ps)
{
	const p3d_emitter_cfg* cfg = symbol->GetEmitterCfg();
	
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

	ps->components.reserve(cfg->symbol_count);
	for (int i = 0; i < cfg->symbol_count; ++i) 
	{
		const p3d_symbol& p_symbol = cfg->symbols[i];

		PackParticle3D::Component comp;

		comp.count = p_symbol.count;

		comp.scale_start = p_symbol.scale_start;
		comp.scale_end = p_symbol.scale_end;

		comp.angle = p_symbol.angle;
		comp.angle_var = p_symbol.angle_var;

		comp.col_mul = s2::Color((int)(p_symbol.col_mul.r * 255), (int)(p_symbol.col_mul.g * 255), 
			(int)(p_symbol.col_mul.b * 255), (int)(p_symbol.col_mul.a * 255)).ToABGR();
		comp.col_add = s2::Color((int)(p_symbol.col_add.r * 255), (int)(p_symbol.col_add.g * 255), 
			(int)(p_symbol.col_add.b * 255), (int)(p_symbol.col_add.a * 255)).ToABGR();
		comp.alpha_start = p_symbol.alpha_start * 255.0f + 0.5f;
		comp.alpha_end = p_symbol.alpha_end * 255.0f + 0.5f;

		ee::Symbol* symbol = static_cast<ee::Symbol*>(p_symbol.ud);
		comp.node = PackNodeFactory::Instance()->Create(symbol);

		ps->components.push_back(comp);
	}
}

}