#include "TestMulti.h"

#include <rvg.h>
#include <shaderlab.h>
#include <sprite2/ImageSprite.h>
#include <sprite2/DrawNode.h>
#include <glue/RenderContext.h>
#include <glue/ImageSymbol.h>

namespace test
{

void TestMulti::Init()
{
	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->CreateContext(4096);
	sl::RenderContext* sl_rc = sl_mgr->GetContext();
	sl_mgr->CreateShader(sl::SPRITE2, new sl::Sprite2Shader(sl_rc));
	sl_mgr->CreateShader(sl::SHAPE2, new sl::Shape2Shader(sl_rc));
	glue::RenderContext::Instance()->SetCamera(0, 0, 1, 1);

	s2::Symbol* sym = new glue::ImageSymbol("coin_00.png");
	{
		s2::Sprite* spr = new s2::ImageSprite(sym);
		spr->SetPosition(sm::vec2(0, 0));
		m_sprites.push_back(spr);
	}
	{
		s2::Sprite* spr = new s2::ImageSprite(sym);
		spr->SetPosition(sm::vec2(100, 100));
		m_sprites.push_back(spr);
	}
	{
		s2::Sprite* spr = new s2::ImageSprite(sym);
		spr->SetPosition(sm::vec2(-100, -100));
		m_sprites.push_back(spr);
	}
}

void TestMulti::Resize(int width, int height)
{
	glue::RenderContext::Instance()->OnSize(width, height);
}

void TestMulti::Draw() const
{
	//////////////////////////////////////////////////////////////////////////
	
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		s2::DrawNode::Draw(m_sprites[i]);
	}

	//////////////////////////////////////////////////////////////////////////

	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->SetShader(sl::SHAPE2);
	sl::Shape2Shader* sl_shader = static_cast<sl::Shape2Shader*>(sl_mgr->GetShader());

	sl_shader->SetColor(0xff0000ff);
	rvg_point_size(5);
	rvg_point(0, 0);
	sl_shader->SetColor(0xffff00ff);
	rvg_line(-100, -100, -50, -80);
	sl_shader->SetColor(0xffffffff);
	rvg_rect(10, 10, 100, 100, true);

	rvg_circle(100, -100, 50, false, 16);

	sl_shader->SetColor(0xff00ffff);
	rvg_circle(200, -100, 50, true, 32);

	//////////////////////////////////////////////////////////////////////////
}

void TestMulti::Update()
{
}

}