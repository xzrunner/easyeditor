#include "Particle2DBuilder.h"
#include "PackParticle2D.h"
#include "PackNodeFactory.h"

#include <ee/Visitor.h>
#include <ee/Exception.h>

#include <easyparticle2d.h>

#include <ps_2d.h>

namespace erespacker
{

Particle2DBuilder::Particle2DBuilder(ExportNameSet& export_set)
	: m_export_set(export_set)
{
}

Particle2DBuilder::~Particle2DBuilder()
{
	std::map<const eparticle2d::Symbol*, const PackParticle2D*>::iterator
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		delete itr->second;
	}
}

void Particle2DBuilder::Traverse(ee::Visitor& visitor) const
{
	std::map<const eparticle2d::Symbol*, const PackParticle2D*>::const_iterator 
		itr = m_map_data.begin();
	for ( ; itr != m_map_data.end(); ++itr) {
		bool has_next;
		visitor.Visit(const_cast<PackParticle2D*>(itr->second), has_next);
		if (!has_next) {
			break;
		}
	}
}

const IPackNode* Particle2DBuilder::Create(const eparticle2d::Symbol* symbol)
{
	std::map<const eparticle2d::Symbol*, const PackParticle2D*>::iterator 
		itr = m_map_data.find(symbol);
	if (itr != m_map_data.end()) {
		return itr->second;
	}

	PackParticle2D* node = new PackParticle2D;
	Load(symbol, node);
	m_map_data.insert(std::make_pair(symbol, node));
	return node;
}

void Particle2DBuilder::Load(const eparticle2d::Symbol* symbol, PackParticle2D* ps)
{
	m_export_set.LoadExport(symbol, ps);

	const p2d_emitter_cfg* cfg = symbol->GetEmitterCfg();

	ps->emission_time = cfg->emission_time;
	ps->count = cfg->count;

	ps->life = cfg->life;
	ps->life_var = cfg->life_var;

	ps->position.x = cfg->position.x;
	ps->position.y = cfg->position.y;
	ps->position_var.x = cfg->position_var.x;
	ps->position_var.y = cfg->position_var.y;

	ps->direction = cfg->direction;
	ps->direction_var = cfg->direction_var;

	ps->mode_type = cfg->mode_type;
	if (ps->mode_type == P2D_MODE_GRAVITY)
	{
		ps->A.gravity.x = cfg->mode.A.gravity.x;
		ps->A.gravity.y = cfg->mode.A.gravity.y;

		ps->A.speed = cfg->mode.A.speed;
		ps->A.speed_var = cfg->mode.A.speed_var;

		ps->A.tangential_accel = cfg->mode.A.tangential_accel;
		ps->A.tangential_accel_var = cfg->mode.A.tangential_accel_var;
		ps->A.radial_accel = cfg->mode.A.radial_accel;
		ps->A.radial_accel_var = cfg->mode.A.radial_accel_var;

		ps->A.rotation_is_dir = cfg->mode.A.rotation_is_dir;
	}
	else if (ps->mode_type == P2D_MODE_RADIUS)
	{
		ps->B.start_radius = cfg->mode.B.start_radius;
		ps->B.start_radius_var = cfg->mode.B.start_radius_var;
		ps->B.end_radius = cfg->mode.B.end_radius;
		ps->B.end_radius_var = cfg->mode.B.end_radius_var;

		ps->B.direction_delta = cfg->mode.B.direction_delta;
		ps->B.direction_delta_var = cfg->mode.B.direction_delta_var;
	}
	else if (ps->mode_type == P2D_MODE_SPD_COS)
	{
		ps->C.speed = cfg->mode.C.speed;
		ps->C.speed_var = cfg->mode.C.speed_var;

		ps->C.cos_amplitude = cfg->mode.C.cos_amplitude;
		ps->C.cos_amplitude_var = cfg->mode.C.cos_amplitude_var;
		ps->C.cos_frequency = cfg->mode.C.cos_frequency;
		ps->C.cos_frequency_var = cfg->mode.C.cos_frequency_var;
	}
	else
	{
		throw ee::Exception("Particle2DBuilder::Load unknown mode type, filepath: %s", symbol->GetFilepath().c_str());
	}


	ps->components.reserve(cfg->symbol_count);
	for (int i = 0; i < cfg->symbol_count; ++i) {
		const p2d_symbol& p_symbol = cfg->symbols[i];

		PackParticle2D::Component comp;

		comp.angle_start = p_symbol.angle_start;
		comp.angle_end = p_symbol.angle_end;

		comp.scale_start = p_symbol.scale_start;
		comp.scale_end = p_symbol.scale_end;

		comp.mul_col_start = s2::Color((int)(p_symbol.mul_col_start.r * 255), (int)(p_symbol.mul_col_start.g * 255), 
			(int)(p_symbol.mul_col_start.b * 255), (int)(p_symbol.mul_col_start.a * 255)).ToABGR();
		comp.mul_col_end = s2::Color((int)(p_symbol.mul_col_end.r * 255), (int)(p_symbol.mul_col_end.g * 255), 
			(int)(p_symbol.mul_col_end.b * 255), (int)(p_symbol.mul_col_end.a * 255)).ToABGR();

		comp.add_col_start = s2::Color((int)(p_symbol.add_col_start.r * 255), (int)(p_symbol.add_col_start.g * 255), 
			(int)(p_symbol.add_col_start.b * 255), (int)(p_symbol.add_col_start.a * 255)).ToABGR();
		comp.add_col_end = s2::Color((int)(p_symbol.add_col_end.r * 255), (int)(p_symbol.add_col_end.g * 255), 
			(int)(p_symbol.add_col_end.b * 255), (int)(p_symbol.add_col_end.a * 255)).ToABGR();

		ee::Symbol* symbol = static_cast<ee::Symbol*>(p_symbol.ud);
		comp.node = PackNodeFactory::Instance()->Create(symbol);

		ps->components.push_back(comp);
	}
}

}