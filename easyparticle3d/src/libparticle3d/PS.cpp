#include "PS.h"
#include "ParticleSystem.h"

#include <ps_3d.h>
#include <ps_3d_sprite.h>
#include <ps_3d_buffer.h>

#include <ee/SpriteRenderer.h>

#include <time.h>
#include <assert.h>

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

	ee::SceneNodeMgr::Instance()->Add(this);
}

bool PS::Update(float dt)
{
	UpdateTime();
	return p3d_buffer_update(GetTime());
}

void PS::Draw() const
{
	p3d_buffer_draw(0, 0, 1);
}

void PS::BufferClear()
{
	p3d_buffer_clear();
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
	assert(ud);
	const RenderParams* rp = (static_cast<const RenderParams*>(ud));
	assert(rp->p3d);

	ee::Symbol* sym = static_cast<ee::Symbol*>(symbol);

	ee::RenderParams trans;
	memcpy(&trans.color.multi, mul_col, sizeof(*mul_col));
	memcpy(&trans.color.add, add_col, sizeof(*add_col));
	trans.color.multi = col_mul(trans.color.multi, rp->ct.multi);
	trans.color.add = col_add(trans.color.add, rp->ct.add);
	// todo color trans

	sm::mat4 mt = rp->mat;
	if (!rp->p3d->local_mode_draw) {
		mt.x[0] = mat[0];
		mt.x[1] = mat[1];
		mt.x[4] = mat[2];
		mt.x[5] = mat[3];
		mt.x[12]= mat[4];
		mt.x[13]= mat[5];
	}
	trans.mt = mt;

	ee::SpriteRenderer::Draw(sym, trans, sm::vec2(x, y), angle, scale, scale, 0, 0);

	// todo bind
	// 	if (p->bind_ps) {
	// 		sm::mat4 _mt;
	// 		_mt.translate(p->pos.x, p->pos.y);
	// 		Draw(p->bind_ps, _mt, recorder);
	// 	}

	// todo record
	// 	AnimRecorder* curr_record = m_anim_recorder ? m_anim_recorder : recorder;
	// 	if (curr_record) {
	// 		sm::vec2 fixed = ee::Math2D::TransVector(pos, _mt);
	// 		curr_record->AddItem(symbol->GetFilepath(), fixed.x, fixed.y, p->angle, s, mul_col, add_col);
	// 	}
}

static void 
add_func(p3d_particle* p, void* ud)
{
//  	ParticleSystem* ps = (ParticleSystem*)ud;
//  	const sm::vec2& pos = ps->GetPosition();
//  	p->init_pos.x = pos.x;
//  	p->init_pos.y = pos.y;
}

static void 
remove_func(p3d_particle* p, void* ud)
{
	ParticleSystem* ps = (ParticleSystem*)ud;
	if (ps) {
		ps->RemoveFromInvertRecord(p);
	}
}

static void
update_srt_func(void* params, float x, float y, float scale) {
	RenderParams* rp = static_cast<RenderParams*>(params);
	rp->mat.Translate(x, y, 0);
	rp->mat.Scale(scale, scale, 1);
}

static void
buf_remove_func(struct p3d_sprite* spr) {
}

static void
create_draw_params_func(struct p3d_sprite* spr) {
	RenderParams* rp = new RenderParams;
	rp->p3d = spr;
	spr->draw_params = rp;
}

void 
release_draw_params_func(struct p3d_sprite* spr) {
	RenderParams* rp = static_cast<RenderParams*>(spr->draw_params);
	delete rp;
	spr->draw_params = NULL;
}

void PS::Init()
{
	p3d_init();
	p3d_regist_cb(render_func, add_func, remove_func);	
	p3d_buffer_init(update_srt_func, buf_remove_func);
	p3d_sprite_init(create_draw_params_func, release_draw_params_func);
}

}