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

	s2::RenderParams params;

	params.color.mul.r = (int)(mul_col->r * 255.0f + 0.5f);
	params.color.mul.g = (int)(mul_col->g * 255.0f + 0.5f);
	params.color.mul.b = (int)(mul_col->b * 255.0f + 0.5f);
	params.color.mul.a = (int)(mul_col->a * 255.0f + 0.5f);

	params.color.add.r = (int)(add_col->r * 255.0f + 0.5f);
	params.color.add.g = (int)(add_col->g * 255.0f + 0.5f);
	params.color.add.b = (int)(add_col->b * 255.0f + 0.5f);
	params.color.add.a = (int)(add_col->a * 255.0f + 0.5f);

	params.color.mul = params.color.mul * rp->ct.mul;
	params.color.add = params.color.add + rp->ct.add;

	// todo color trans

	if (rp->p3d->local_mode_draw) {
		params.mt = rp->mat;
	} else {
		sm::mat4 _mat;
		_mat.x[0] = mat[0];
		_mat.x[1] = mat[1];
		_mat.x[4] = mat[2];
		_mat.x[5] = mat[3];
		_mat.x[12]= mat[4];
		_mat.x[13]= mat[5];
		params.mt = _mat * rp->mat;
	}

	ee::SpriteRenderer::Draw(sym, params, sm::vec2(x, y), angle, scale, scale, 0, 0);

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