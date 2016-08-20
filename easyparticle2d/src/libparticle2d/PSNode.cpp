#include "PSNode.h"
#include "ParticleSystem.h"

#include <ee/SpriteRenderer.h>
#include <ee/Symbol.h>

#include <ps_2d.h>

#include <time.h>

namespace eparticle2d
{

PSNode* PSNode::m_instance = NULL;

PSNode* PSNode::Instance()
{
	if (!m_instance) {
		m_instance = new PSNode();
	}
	return m_instance;
}

PSNode::PSNode()
	: m_time(0)
{
	Init();
}

void PSNode::UpdateTime()
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
render_func(void* sym, float* mat, float x, float y, float angle, float scale, 
            struct ps_color* mul_col, struct ps_color* add_col, const void* ud)
{
	sm::mat4 mt;
	if (ud) {
		mt = *(sm::mat4*)ud;
	} else {
		mt.x[0] = mat[0];
		mt.x[1] = mat[1];
		mt.x[4] = mat[2];
		mt.x[5] = mat[3];
		mt.x[12] = mat[4];
		mt.x[13] = mat[5];		
	}

	ee::Symbol* ee_sym = static_cast<ee::Symbol*>(sym);

	s2::RenderParams params;
	params.mt = mt;
	memcpy(&params.color.mul, mul_col, sizeof(*mul_col));
	memcpy(&params.color.add, add_col, sizeof(*add_col));

	ee::SpriteRenderer::Instance()->Draw(ee_sym, params, sm::vec2(x, y), angle, 
		sm::vec2(scale, scale), sm::vec2(0, 0));
}

void PSNode::Init()
{
	p2d_init();
	p2d_regist_cb(render_func);	
}

}