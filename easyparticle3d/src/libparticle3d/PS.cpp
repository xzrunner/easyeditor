#include "PS.h"
#include "ParticleSystem.h"

#include <ps_3d.h>
#include <ps_3d_buffer.h>

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
	: m_time(0)
{
	Init();

	d2d::SceneNodeMgr::Instance()->Add(this);
}

void PS::Update(float dt)
{
	p3d_buffer_update(dt);
}

void PS::Draw() const
{
	p3d_buffer_draw();
}

void PS::UpdateTime()
{
	static clock_t last = 0;
	if (last == 0) {
		last = clock();
	} else {
		clock_t curr = clock();
		if (curr != last) {
			float dt = (float)(curr - last) / CLOCKS_PER_SEC;
			m_time += dt;
			last = curr;
		}
	}
}

static void 
render_func(void* symbol, float* mat, float x, float y, float angle, float scale, 
            struct ps_color4f* mul_col, struct ps_color4f* add_col, const void* ud)
{
	d2d::Matrix mt;
	if (ud) {
		mt = *(d2d::Matrix*)ud;
	} else {
		float* m = (float*)mt.getElements();
		m[0] = mat[0];
		m[1] = mat[1];
		m[4] = mat[2];
		m[5] = mat[3];
		m[12] = mat[4];
		m[13] = mat[5];		
	}

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
//  	ParticleSystem* ps = (ParticleSystem*)ud;
//  	const d2d::Vector& pos = ps->GetPosition();
//  	p->init_pos.x = pos.x;
//  	p->init_pos.y = pos.y;
}

static void 
remove_func(p3d_particle* p, void* ud)
{
	ParticleSystem* ps = (ParticleSystem*)ud;
	ps->RemoveFromInvertRecord(p);
}

void PS::Init()
{
	p3d_init();
	p3d_regist_cb(render_func, add_func, remove_func);	

	p3d_buffer_init();
}

}