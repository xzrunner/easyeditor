#include "TestSprite25.h"

#include <rvg.h>
#include <shaderlab.h>

namespace test
{

void TestSprite25::Init()
{
	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->CreateContext(4096);
	sl::RenderContext* sl_rc = sl_mgr->GetContext();
	sl_mgr->CreateShader(sl::SPRITE3, new sl::Sprite3Shader(sl_rc));
//	sl_mgr->CreateShader(sl::SHAPE, new sl::Shape2Shader(sl_rc));

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

void TestSprite25::Resize(int width, int height)
{
	glue::RenderContext::Instance()->OnSize(width, height);
	glue::RenderContext::Instance()->SetCamera(0, 0, 1, 1);
}

void TestSprite25::Draw() const
{
	//////////////////////////////////////////////////////////////////////////
	
// 	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
// 		glue::Sprite* spr = m_sprites[i];
// 		sm_mat4 mt = spr->GetTransMatrix();
// 		spr->GetSymbol().Draw(mt);	
// 	}

	{
		sm_mat4 mt = m_sprites[0]->GetTransMatrix();
		static_cast<const glue::ImageSymbol*>(&m_sprites[0]->GetSymbol())->Draw25(mt);	
	}
// 	{
// 		sm_mat4 mt = m_sprites[1]->GetTransMatrix();
// 		m_sprites[1]->GetSymbol().ModelDraw(mt);	
// 	}
// 	{
// 		sm_mat4 mt = m_sprites[2]->GetTransMatrix();
// 		m_sprites[2]->GetSymbol().Draw(mt);	
// 	}
//	//////////////////////////////////////////////////////////////////////////
//
//	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
//	sl_mgr->SetShader(sl::SHAPE);
//	sl::Shape2Shader* sl_shader = static_cast<sl::Shape2Shader*>(sl_mgr->GetShader());
//
//	sl_shader->SetColor(0xff0000ff);
//	rvg_point_size(5);
//	rvg_point(0, 0);
//// 	sl_shape_color(0xffff00ff);
//// 	rvg_line(-100, -100, -50, -80);
//// 	sl_shape_color(0xffffffff);
//// 	rvg_rect(10, 10, 100, 100, true);
//// 
//// 	rvg_circle(100, -100, 50, false, 16);
//// 
//// 	sl_shape_color(0xff00ffff);
//// 	rvg_circle(200, -100, 50, true, 32);
//
//	//////////////////////////////////////////////////////////////////////////
}

void TestSprite25::Update()
{
}

}