#include "PS.h"
#include "ParticleSystem.h"

#include <ee/RenderColor.h>
#include <ee/SpriteRenderer.h>

#include <ps_2d.h>

#include <time.h>

namespace eparticle2d
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

	ee::Symbol* sym = static_cast<ee::Symbol*>(symbol);

	ee::RenderParams trans;
	trans.mt = mt;
	memcpy(&trans.color.multi, mul_col, sizeof(*mul_col));
	memcpy(&trans.color.add, add_col, sizeof(*add_col));

	ee::SpriteRenderer::Draw(sym, trans, sm::vec2(x, y), angle, scale, scale, 0, 0);
}

void PS::Init()
{
	p2d_init();
	p2d_regist_cb(render_func);	
}

}