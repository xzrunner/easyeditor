#include "TestImage25.h"

#include <rvg.h>
#include <sl_shape.h>

namespace test
{

void TestImage25::Init()
{
	glue::ShaderMgr::Instance()->Init();

	glue::Symbol* sym = new glue::ImageSymbol("coin_00.png");
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

void TestImage25::Resize(int width, int height)
{
	glue::RenderContext::Instance()->OnSize(width, height);
}

void TestImage25::Draw() const
{
	//////////////////////////////////////////////////////////////////////////
	
// 	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
// 		glue::Sprite* spr = m_sprites[i];
// 		glue::mat4 mt = spr->GetTransMatrix();
// 		spr->GetSymbol().Draw(mt);	
// 	}

	{
		glue::mat4 mt = m_sprites[0]->GetTransMatrix();
		static_cast<const glue::ImageSymbol*>(&m_sprites[0]->GetSymbol())->ModelDraw(mt);	
	}
// 	{
// 		glue::mat4 mt = m_sprites[1]->GetTransMatrix();
// 		m_sprites[1]->GetSymbol().ModelDraw(mt);	
// 	}
// 	{
// 		glue::mat4 mt = m_sprites[2]->GetTransMatrix();
// 		m_sprites[2]->GetSymbol().Draw(mt);	
// 	}
	//////////////////////////////////////////////////////////////////////////

	glue::ShaderMgr* mgr = glue::ShaderMgr::Instance();
	mgr->SetShader(glue::ShaderMgr::SHAPE);

	sl_shape_color(0xff0000ff);
	rvg_point_size(5);
	rvg_point(0, 0);
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
}

void TestImage25::Update()
{
}

}