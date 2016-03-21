#include "TestShape.h"

#include <glue.h>
#include <rvg.h>
#include <sl_shape.h>

namespace test
{

void TestShape::Init()
{
	glue::ShaderMgr::Instance()->Init();
}

void TestShape::Resize(int width, int height)
{
	glue::ShaderMgr::Instance()->OnSize(width, height);
}

void TestShape::Draw() const
{
	glue::ShaderMgr* mgr = glue::ShaderMgr::Instance();
	mgr->SetShader(glue::ShaderMgr::SHAPE);

	//////////////////////////////////////////////////////////////////////////

// 	sl_shape_color(0xff0000ff);
// 	rvg_point_size(5);
// 	rvg_point(0, 0);
// 	sl_shape_color(0xffff00ff);
// 	rvg_line(-100, -100, -50, -80);
// 	sl_shape_color(0xffffffff);
// 	rvg_rect(10, 10, 100, 100, true);
// 
// 	rvg_circle(100, -100, 50, false, 16);
// 
// 	sl_shape_color(0xff00ffff);
// 	rvg_circle(200, -100, 50, true, 32);

	//////////////////////////////////////////////////////////////////////////

//  	float line1[] = {1, 1, 100, 100, 100, 0};
//  	sl_shape_color(0xffffffff);
//  	rvg_polyline(line1, 3, true);
//  
//  	float line2[] = {200, 200, 250, 225, 250, -200};
//  	sl_shape_color(0xffff00ff);
//  	rvg_polyline(line2, 3, true);
// 
// 	rvg_rect(0, -100, 50, -150, true);
// 	rvg_rect(-25, -100, -50, -150, false);
// 
//  	float line3[] = {-1, -1, -100, -100, -100, -150};
//  	sl_shape_color(0x00ffffff);
//  	rvg_polyline(line3, 3, false);
// 
// 	rvg_circle(0, 0, 50, true, 5);
// 
// 	rvg_circle(-200, -100, 50, false, 16);
// 
// 	rvg_circle( 100, -100, 50, true, 9);

	//////////////////////////////////////////////////////////////////////////

	rvg_line_style(LS_DEFAULT);
	rvg_line(0, 0, 100, 100);

	rvg_line_style(LS_DOT);
	rvg_line(10, 0, 10, -100);

	rvg_line_style(LS_DASH);
	rvg_line(-10, 0, -10, -100);

	rvg_line_style(LS_DOT_DASH);
	rvg_line(-50, 0, -50, -100);

	rvg_line_style(LS_DEFAULT);
	rvg_line(-50, -50, -100, -100);
}

void TestShape::Update()
{

}

}