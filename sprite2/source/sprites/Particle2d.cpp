#include "Particle2d.h"
#include "S2_Symbol.h"
#include "RenderParams.h"
#include "Color.h"
#include "DrawNode.h"

#include <SM_Matrix.h>
#include <ps_2d.h>

namespace s2
{

SINGLETON_DEFINITION(Particle2d)

Particle2d::Particle2d()
	: m_time(0)
{
	Init();
}

void Particle2d::Update(float dt)
{
	m_time += dt;
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

	Symbol* s2_sym = static_cast<Symbol*>(sym);

	RenderParams params;
	params.mt = mt;
	memcpy(&params.color.mul, mul_col, sizeof(*mul_col));
	memcpy(&params.color.add, add_col, sizeof(*add_col));

	DrawNode::Draw(s2_sym, params, sm::vec2(x, y), angle, sm::vec2(scale, scale), sm::vec2(0, 0));
}

void Particle2d::Init()
{
	p2d_init();
	p2d_regist_cb(render_func);	
}

}