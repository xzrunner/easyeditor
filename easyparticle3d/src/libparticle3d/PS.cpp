#include "PS.h"
#include "ParticleSystem.h"

#include <drag2d.h>

#include <particle3d.h>

namespace eparticle3d
{

PS* PS::m_instance = NULL;

PS* PS::Instance()
{
	if (!m_instance) {
		m_instance = new PS();
	}
	return m_instance;
}

PS::PS()
{
	Init();
}

static void 
render_func(void* symbol, float x, float y, float angle, float scale, 
            struct ps_color4f* mul_col, struct ps_color4f* add_col, const void* ud)
{
	d2d::Matrix mt = *(d2d::Matrix*)ud;
	//	mt.translate(p->init_pos.x, p->init_pos.y);

	d2d::ISymbol* sym = static_cast<d2d::ISymbol*>(symbol);
	d2d::ColorTrans color;
	memcpy(&color.multi, mul_col, sizeof(*mul_col));
	memcpy(&color.add, add_col, sizeof(*add_col));

	d2d::SpriteRenderer::Instance()->Draw(sym, mt, d2d::Vector(x, y), angle, scale, scale, 0, 0, color);

	// todo bind
	// 	if (p->bind_ps) {
	// 		d2d::Matrix _mt;
	// 		_mt.translate(p->pos.x, p->pos.y);
	// 		Draw(p->bind_ps, _mt, recorder);
	// 	}

	// todo record
	// 	AnimRecorder* curr_record = m_anim_recorder ? m_anim_recorder : recorder;
	// 	if (curr_record) {
	// 		d2d::Vector fixed = d2d::Math::transVector(pos, _mt);
	// 		curr_record->AddItem(symbol->GetFilepath(), fixed.x, fixed.y, p->angle, s, mul_col, add_col);
	// 	}
}

static void 
add_func(p3d_particle* p, void* ud)
{
	ParticleSystem* ps = (ParticleSystem*)ud;
	const d2d::Vector& pos = ps->GetPosition();
	p->init_pos.x = pos.x;
	p->init_pos.y = pos.y;
}

static void 
remove_func(p3d_particle* p, void* ud)
{
	ParticleSystem* ps = (ParticleSystem*)ud;
	ps->RemoveFromInvertRecord(p);
}

void PS::Init()
{
	p3d_init(render_func, add_func, remove_func);	
}

}