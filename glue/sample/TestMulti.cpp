#include "TestMulti.h"

#include <rvg_render.h>
#include <sl_shape.h>

namespace test
{

void TestMulti::Init()
{
	glue::ShaderMgr::Instance()->Init();

	glue::Symbol* sym = new glue::Symbol("coin_00.png");
	{
		glue::Sprite* spr = new glue::Sprite(sym);
		spr->SetPositioin(glue::vec2(0, 0));
		m_sprites.push_back(spr);
	}
	{
		glue::Sprite* spr = new glue::Sprite(sym);
		spr->SetPositioin(glue::vec2(100, 100));
		m_sprites.push_back(spr);
	}
	{
		glue::Sprite* spr = new glue::Sprite(sym);
		spr->SetPositioin(glue::vec2(-100, -100));
		m_sprites.push_back(spr);
	}
}

void TestMulti::Resize(int width, int height)
{
	glue::ShaderMgr::Instance()->OnSize(width, height);
}

void TestMulti::Draw() const
{
	//////////////////////////////////////////////////////////////////////////
	
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		glue::Sprite* spr = m_sprites[i];
		glue::mat4 mt = spr->GetTransMatrix();
		spr->GetSymbol().Draw(mt);	
	}

	glue::ShaderMgr::Instance()->Flush();

	//////////////////////////////////////////////////////////////////////////

	glue::ShaderMgr::Instance()->ShapeDraw();

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

	//////////////////////////////////////////////////////////////////////////
}

void TestMulti::Update()
{
}

}