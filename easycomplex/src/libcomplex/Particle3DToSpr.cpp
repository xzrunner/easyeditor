#include "Particle3DToSpr.h"
#include "NodeToSprite.h"

#include <easyparticle3d.h>
#include <particle3d.h>

namespace ecomplex
{

d2d::ISprite* Particle3DToSpr::Trans(const libcoco::PackParticle3D* p3d)
{
	p3d_ps_config* cfg = LoadConfig(p3d);

	eparticle3d::ParticleSystem* ps = new eparticle3d::ParticleSystem(eparticle3d::PARTICLE_CAP, cfg);

	eparticle3d::Symbol* symbol = new eparticle3d::Symbol;
	symbol->SetPS(ps);
	return new eparticle3d::Sprite(symbol);
}

p3d_ps_config* Particle3DToSpr::LoadConfig(const libcoco::PackParticle3D* p3d)
{
	int sz = sizeof(p3d_ps_config) + sizeof(p3d_symbol) * eparticle3d::MAX_COMPONENTS;
	p3d_ps_config* cfg = (p3d_ps_config*) operator new(sz);
	memset(cfg, 0, sz);

	cfg->emission_time = p3d->emission_time;
	cfg->count = p3d->count;

	cfg->life = p3d->life;
	cfg->life_var = p3d->life_var;

	cfg->hori = p3d->hori;
	cfg->hori_var = p3d->hori_var;
	cfg->vert = p3d->vert;
	cfg->vert_var = p3d->vert_var;

	cfg->spd = p3d->spd;
	cfg->spd_var = p3d->spd_var;
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

	cfg->bounce = p3d->bounce;

	cfg->start_radius = p3d->start_radius;
	cfg->is_start_radius_3d = p3d->is_start_radius_3d;

	cfg->orient_to_movement = p3d->orient_to_movement;

	// todo dir
	cfg->dir.x = 0;
	cfg->dir.y = 0;
	cfg->dir.z = 1;

	cfg->symbol_count = p3d->components.size();
	cfg->symbols = (p3d_symbol*)(cfg+1);
	for (int i = 0, n = p3d->components.size(); i < n; ++i)
	{
		const libcoco::PackParticle3D::Component& src = p3d->components[i];
		p3d_symbol& dst = cfg->symbols[i];

		dst.scale_start = src.scale_start;
		dst.scale_end = src.scale_end;

		dst.angle = src.angle;
		dst.angle_var = src.angle_var;

		d2d::Colorf col_mul = d2d::transColor(src.col_mul, d2d::PT_ARGB),
			col_add = d2d::transColor(src.col_add, d2d::PT_ARGB);
		memcpy(&dst.col_mul.r, &col_mul.r, sizeof(col_mul));
		memcpy(&dst.col_add.r, &col_add.r, sizeof(col_add));
		
		dst.alpha_start = src.alpha_start;
		dst.alpha_end = src.alpha_end;

		// todo bind ps

		d2d::ISprite* spr = NodeToSprite::Trans(src.node);
		dst.ud = const_cast<d2d::ISymbol*>(&spr->GetSymbol());
	}

	return cfg;
}

}