#include "TestMulti.h"

#include <rvg_render.h>
#include <sl_sprite.h>
#include <sl_shape.h>
#include <sl_shader.h>

namespace test
{

void TestMulti::Init()
{
	sl_sprite_init();
	sl_sprite_modelview(0, 0, 1, 1);

	sl_shape_init();
	sl_shape_modelview(0, 0, 1, 1);

	glue::Symbol* sym = new glue::Symbol("coin_00.png");
	m_spr = new glue::Sprite(sym);
}

void TestMulti::Resize(int width, int height)
{
	sl_sprite_projection(width, height);
	sl_shape_projection(width, height);
}

void TestMulti::Draw() const
{
	sl_sprite_bind();

	glue::mat4 mt = m_spr->GetTransMatrix();
	m_spr->GetSymbol().Draw(mt);

	//////////////////////////////////////////////////////////////////////////

 	sl_shape_bind();

	sl_shape_color(0xff0000ff);
	rvg_point(0, 0, 5);
	sl_shape_color(0xffff00ff);
	rvg_line(-100, -100, -50, -80);
	sl_shape_color(0xffffffff);
	rvg_rect(10, 10, 100, 100, true);

	rvg_circle(100, -100, 50, false);

	sl_shape_color(0xff00ffff);
	rvg_set_circle_segments(32);
	rvg_circle(200, -100, 50, true);
}

void TestMulti::Update()
{
}

}