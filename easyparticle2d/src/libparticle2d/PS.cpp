#include "PS.h"
#include "ParticleSystem.h"

#include <drag2d.h>

#include <particle2d.h>

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
{
	Init();
}

static void 
render_func(void* symbol, float x, float y, float angle, float scale, 
            struct ps_color4f* mul_col, struct ps_color4f* add_col, const void* ud)
{
	d2d::Matrix mt = *(d2d::Matrix*)ud;

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