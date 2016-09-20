#include "Particle2DToSpr.h"
#include "NodeToSprite.h"

#include <gum/trans_color.h>

#include <easyparticle2d.h>
#include <ps_2d.h>

namespace ecomplex
{

ee::Sprite* Particle2DToSpr::Trans(const erespacker::PackParticle2D* p2d)
{
	eparticle2d::Symbol* sym = new eparticle2d::Symbol;
	sym->SetEmitterCfg(LoadConfig(p2d));
	return new eparticle2d::Sprite(sym);
}

p2d_emitter_cfg* Particle2DToSpr::LoadConfig(const erespacker::PackParticle2D* p2d)
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

	cfg->sym_count = p2d->components.size();
	cfg->syms = (p2d_symbol*)(cfg+1);
	for (int i = 0, n = p2d->components.size(); i < n; ++i)
	{
		const erespacker::PackParticle2D::Component& src = p2d->components[i];
		p2d_symbol& dst = cfg->syms[i];

		dst.angle_start = src.angle_start;
		dst.angle_end = src.angle_end;

		dst.scale_start = src.scale_start;
		dst.scale_end = src.scale_end;

		s2::Color mul_col_begin = int2color(src.mul_col_begin, gum::ARGB),
			      mul_col_end	= int2color(src.mul_col_end, gum::ARGB),
				  add_col_begin = int2color(src.add_col_begin, gum::ARGB),
				  add_col_end	= int2color(src.add_col_end, gum::ARGB);

		memcpy(&dst.mul_col_begin.r, &mul_col_begin.r, sizeof(mul_col_begin));
		memcpy(&dst.mul_col_end.r, &mul_col_end.r, sizeof(mul_col_end));
		memcpy(&dst.add_col_begin.r, &add_col_begin.r, sizeof(add_col_begin));
		memcpy(&dst.add_col_end.r, &add_col_end.r, sizeof(add_col_end));
		
		ee::Sprite* spr = NodeToSprite::Trans(src.node);
		dst.ud = dynamic_cast<ee::Symbol*>(spr->GetSymbol());
	}

	return cfg;
}

}