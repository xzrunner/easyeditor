#include "Particle2DToSpr.h"
#include "NodeToSprite.h"

#include <easyparticle2d.h>
#include <ps_2d.h>

namespace ecomplex
{

ee::Sprite* Particle2DToSpr::Trans(const librespacker::PackParticle2D* p2d)
{
	eparticle2d::Symbol* symbol = new eparticle2d::Symbol;
	symbol->SetEmitterCfg(LoadConfig(p2d));
	return new eparticle2d::Sprite(symbol);
}

p2d_emitter_cfg* Particle2DToSpr::LoadConfig(const librespacker::PackParticle2D* p2d)
{
	int sz = SIZEOF_P2D_EMITTER_CFG + SIZEOF_P2D_SYMBOL * eparticle2d::MAX_COMPONENTS;
	p2d_emitter_cfg* cfg = (p2d_emitter_cfg*) operator new(sz);
	memset(cfg, 0, sz);

	cfg->mode_type = p2d->mode_type;

	if (cfg->mode_type == P2D_MODE_GRAVITY)
	{
		cfg->mode.A.gravity.x = p2d->A.gravity.x;
		cfg->mode.A.gravity.y = p2d->A.gravity.y;

		cfg->mode.A.speed = p2d->A.speed;
		cfg->mode.A.speed_var = p2d->A.speed_var;

		cfg->mode.A.tangential_accel = p2d->A.tangential_accel;
		cfg->mode.A.tangential_accel_var = p2d->A.tangential_accel_var;
		cfg->mode.A.radial_accel = p2d->A.radial_accel;
		cfg->mode.A.radial_accel_var = p2d->A.radial_accel_var;

		cfg->mode.A.rotation_is_dir = p2d->A.rotation_is_dir;
	}
	else if (cfg->mode_type == P2D_MODE_RADIUS)
	{
		cfg->mode.B.start_radius = p2d->B.start_radius;
		cfg->mode.B.start_radius_var = p2d->B.start_radius_var;
		cfg->mode.B.end_radius = p2d->B.end_radius;
		cfg->mode.B.end_radius_var = p2d->B.end_radius_var;

		cfg->mode.B.direction_delta = p2d->B.direction_delta;
		cfg->mode.B.direction_delta_var = p2d->B.direction_delta_var;
	}
	else if (cfg->mode_type == P2D_MODE_SPD_COS)
	{
		cfg->mode.C.speed = p2d->C.speed;
		cfg->mode.C.speed_var = p2d->C.speed_var;

		cfg->mode.C.cos_amplitude = p2d->C.cos_amplitude;
		cfg->mode.C.cos_amplitude_var = p2d->C.cos_amplitude_var;
		cfg->mode.C.cos_frequency = p2d->C.cos_frequency;
		cfg->mode.C.cos_frequency_var = p2d->C.cos_frequency_var;
	}
	else
	{
		throw ee::Exception("Particle2DToSpr::LoadConfig unknown mode type.");
	}

	cfg->emission_time = p2d->emission_time;
	cfg->count = p2d->count;

	cfg->life = p2d->life;
	cfg->life_var = p2d->life_var;

	cfg->position.x = p2d->position.x;
	cfg->position.y = p2d->position.y;
	cfg->position_var.x = p2d->position_var.x;
	cfg->position_var.y = p2d->position_var.y;

	cfg->direction = p2d->direction;
	cfg->direction_var = p2d->direction_var;

	cfg->symbol_count = p2d->components.size();
	cfg->symbols = (p2d_symbol*)(cfg+1);
	for (int i = 0, n = p2d->components.size(); i < n; ++i)
	{
		const librespacker::PackParticle2D::Component& src = p2d->components[i];
		p2d_symbol& dst = cfg->symbols[i];

		dst.angle_start = src.angle_start;
		dst.angle_end = src.angle_end;

		dst.scale_start = src.scale_start;
		dst.scale_end = src.scale_end;

		ee::Colorf 
			col_mul_start = ee::TransColor(src.col_mul_start, ee::PT_ARGB),
			col_mul_end = ee::TransColor(src.col_mul_end, ee::PT_ARGB),
			col_add_start = ee::TransColor(src.col_add_start, ee::PT_ARGB),
			col_add_end = ee::TransColor(src.col_add_end, ee::PT_ARGB);

		memcpy(&dst.col_mul_start.r, &col_mul_start.r, sizeof(col_mul_start));
		memcpy(&dst.col_mul_end.r, &col_mul_end.r, sizeof(col_mul_end));
		memcpy(&dst.col_add_start.r, &col_add_start.r, sizeof(col_add_start));
		memcpy(&dst.col_add_end.r, &col_add_end.r, sizeof(col_add_end));
		
		ee::Sprite* spr = NodeToSprite::Trans(src.node);
		dst.ud = const_cast<ee::Symbol*>(&spr->GetSymbol());
	}

	return cfg;
}

}