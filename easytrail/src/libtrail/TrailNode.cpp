#include "TrailNode.h"

#include <ee/Symbol.h>
#include <ee/SpriteRenderer.h>

#include <mt_2d.h>
#include <shaderlab.h>

#include <time.h>
#include <assert.h>

namespace etrail
{

TrailNode* TrailNode::m_instance = NULL;

TrailNode* TrailNode::Instance()
{
	if (!m_instance) {
		m_instance = new TrailNode();
	}
	return m_instance;
}

TrailNode::TrailNode()
	: m_time(0)
{
	Init();

	ee::SceneNodeMgr::Instance()->Add(this);
}

bool TrailNode::Update(float dt)
{
	return true;
}

void TrailNode::Draw() const
{
	
}

void TrailNode::UpdateTime()
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
render_symbol_func(void* symbol, float x, float y, float angle, float scale, uint8_t* mul_col, uint8_t* add_col, const void* ud)
{
	assert(ud);
	const RenderParams* rp = (static_cast<const RenderParams*>(ud));

	ee::Symbol* sym = static_cast<ee::Symbol*>(symbol);

	s2::RenderParams params;

	memcpy(&params.color.mul.r, mul_col, sizeof(uint8_t) * 4);
	memcpy(&params.color.add.r, add_col, sizeof(uint8_t) * 4);
	params.color.mul = params.color.mul * rp->ct.mul;
	params.color.add = params.color.add + rp->ct.add;

	ee::SpriteRenderer::Draw(sym, params, sm::vec2(x, y), angle - SM_PI * 0.5f, scale, scale);
}

static void 
render_shape_func(const float* positions, const uint32_t* colors, int count, const void* ud)
{
	assert(ud);
	const RenderParams* rp = (static_cast<const RenderParams*>(ud));

	std::vector<uint32_t> t_colors;
	t_colors.resize(count);
	for (int i = 0; i < count; ++i) {
		uint32_t col = colors[i];
		int a = (col >> 24) & 0xff,
			b = (col >> 16) & 0xff,
			g = (col >>  8) & 0xff,
			r = (col) & 0xff;
		a *= rp->ct.mul.a / 255;
		b *= rp->ct.mul.b / 255;
		g *= rp->ct.mul.g / 255;
		r *= rp->ct.mul.r / 255;
		a += rp->ct.add.a;
		b += rp->ct.add.b;
		g += rp->ct.add.g;
		r += rp->ct.add.r;
		t_colors[i] = (a << 24) | (b << 16) | (g << 8) | r;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SHAPE2);
	sl::Shape2Shader* shader = static_cast<sl::Shape2Shader*>(mgr->GetShader());

//	shader->Commit();

	shader->SetType(0x0005);	// todo from rvg_render.c
	shader->Draw(positions, &t_colors[0], count);
}

void TrailNode::Init()
{
	t2d_init();
	t2d_regist_cb(render_symbol_func, render_shape_func);	
}

}