#include "MTrail.h"

#include <ee/Symbol.h>
#include <ee/SpriteRenderer.h>

#include <mt_2d.h>
#include <shaderlab.h>

#include <time.h>
#include <assert.h>

namespace etrail
{

MTrail* MTrail::m_instance = NULL;

MTrail* MTrail::Instance()
{
	if (!m_instance) {
		m_instance = new MTrail();
	}
	return m_instance;
}

MTrail::MTrail()
	: m_time(0)
{
	Init();

	ee::SceneNodeMgr::Instance()->Add(this);
}

bool MTrail::Update(float dt)
{
	return true;
}

void MTrail::Draw() const
{
	
}

void MTrail::UpdateTime()
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
render_symbol_func(void* symbol, float x, float y, float angle, uint8_t* mul_col, uint8_t* add_col, const void* ud)
{
	assert(ud);
	const RenderParams* rp = (static_cast<const RenderParams*>(ud));
	assert(rp->trail);

	ee::Symbol* sym = static_cast<ee::Symbol*>(symbol);

	s2::RenderParams params;

	memcpy(&params.color.mul.r, mul_col, sizeof(uint8_t) * 4);
	memcpy(&params.color.add.r, add_col, sizeof(uint8_t) * 4);
	params.color.mul = params.color.mul * rp->ct.mul;
	params.color.add = params.color.add + rp->ct.add;

	ee::SpriteRenderer::Draw(sym, params, sm::vec2(x, y), angle - SM_PI * 0.5f);
}

static void 
render_shape_func(const float* positions, const uint32_t* colors, int count)
{
	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SHAPE2);
	sl::Shape2Shader* shader = static_cast<sl::Shape2Shader*>(mgr->GetShader());
	shader->SetType(0x0005);	// todo from rvg_render.c
	shader->Draw(positions, colors, count);
}

void MTrail::Init()
{
	t2d_init();
	t2d_regist_cb(render_symbol_func, render_shape_func);	
}

}