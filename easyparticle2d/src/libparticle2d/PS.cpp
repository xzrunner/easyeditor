#include "PS.h"
#include "ParticleSystem.h"

#include <drag2d.h>

#include <ps_2d.h>

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
}

void PS::Init()
{
	p2d_init();
	p2d_regist_cb(render_func);	
}

}