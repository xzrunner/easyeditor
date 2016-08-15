#include "Particle3DToSpr.h"
#include "NodeToSprite.h"

#include <ee/trans_color.h>

#include <easyparticle3d.h>
#include <ps_3d.h>

namespace ecomplex
{

ee::Sprite* Particle3DToSpr::Trans(const erespacker::PackParticle3D* p3d)
{
	eparticle3d::Symbol* symbol = new eparticle3d::Symbol;
	symbol->SetEmitterCfg(LoadConfig(p3d));
	return new eparticle3d::Sprite(symbol);
}

p3d_emitter_cfg* Particle3DToSpr::LoadConfig(const erespacker::PackParticle3D* p3d)
{
	int sz = SIZEOF_P3D_EMITTER_CFG + SIZEOF_P3D_SYMBOL * eparticle3d::MAX_COMPONENTS;
	p3d_emitter_cfg* cfg = (p3d_emitter_cfg*) operator new(sz);
	memset(cfg, 0, sz);

	cfg->blend = p3d->blend;

	cfg->static_mode = p3d->static_mode;

	cfg->emission_time = p3d->emission_time;
	cfg->count = p3d->count;

	cfg->life = p3d->life;
	cfg->life_var = p3d->life_var;

	cfg->hori = p3d->hori;
	cfg->hori_var = p3d->hori_var;
	cfg->vert = p3d->vert;
	cfg->vert_var = p3d->vert_var;

	cfg->radial_spd = p3d->radial_spd;
	cfg->radial_spd_var = p3d->radial_spd_var;
	cfg->tangential_spd = p3d->tangential_spd;
	cfg->tangential_spd_var = p3d->tangential_spd_var;
	cfg->angular_spd = p3d->angular_spd;
	cfg->angular_spd_var = p3d->angular_spd_var;

	cfg->dis_region = p3d->dis_region;
	cfg->dis_region_var = p3d->dis_region_var;
	cfg->dis_spd = p3d->dis_spd;
	cfg->dis_spd_var = p3d->dis_spd_var;

	cfg->gravity = p3d->gravity;

	cfg->linear_acc = p3d->linear_acc;
	cfg->linear_acc_var = p3d->linear_acc_var;

	cfg->fadeout_time = p3d->fadeout_time;

	cfg->ground = p3d->ground;

	cfg->start_radius = p3d->start_radius;
	cfg->start_height = p3d->start_height;

	cfg->orient_to_movement = p3d->orient_to_movement;

	// todo dir
	cfg->dir.x = 0;
	cfg->dir.y = 0;
	cfg->dir.z = 1;

	cfg->symbol_count = p3d->components.size();
	cfg->symbols = (p3d_symbol*)(cfg+1);
	for (int i = 0, n = p3d->components.size(); i < n; ++i)
	{
		const erespacker::PackParticle3D::Component& src = p3d->components[i];
		p3d_symbol& dst = cfg->symbols[i];

		dst.count = cfg->count;

		dst.scale_start = src.scale_start;
		dst.scale_end = src.scale_end;

		dst.angle = src.angle;
		dst.angle_var = src.angle_var;

		s2::Color mul_col_begin = int2color(src.mul_col_begin, ee::PT_ARGB),
			      mul_col_end = int2color(src.mul_col_end, ee::PT_ARGB),
				  add_col_begin = int2color(src.add_col_begin, ee::PT_ARGB),
				  add_col_end = int2color(src.add_col_end, ee::PT_ARGB);
		dst.mul_col_begin = eparticle3d::ColorFromS2(mul_col_begin);
		dst.mul_col_end = eparticle3d::ColorFromS2(mul_col_end);
		dst.add_col_begin = eparticle3d::ColorFromS2(add_col_begin);
		dst.add_col_end = eparticle3d::ColorFromS2(add_col_end);

		// todo bind ps

		ee::Sprite* spr = NodeToSprite::Trans(src.node);
		dst.ud = const_cast<ee::Symbol*>(spr->GetSymbol());
	}

	return cfg;
}

}