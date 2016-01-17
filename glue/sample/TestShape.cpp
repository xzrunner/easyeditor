#include "TestShape.h"

#include <rvg_render.h>
#include <sl_shape.h>

namespace test
{

void TestShape::Init()
{
	sl_shape_init();
	sl_shape_modelview(0, 0, 1, 1);
}

void TestShape::Resize(int width, int height)
{
	sl_shape_projection(width, height);
	sl_shape_bind();
}

void TestShape::Draw() const
{
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

void TestShape::Update()
{

}

}