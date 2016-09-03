#include "TestSprite.h"

#include <sprite2/ImageSprite.h>
#include <sprite2/DrawNode.h>
#include <glue/RenderContext.h>
#include <glue/ImageSymbol.h>
#include <glue/SymbolFactory.h>
#include <shaderlab.h>

namespace test
{

void TestSprite::Init()
{
	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->CreateContext(4096);
	sl::RenderContext* sl_rc = sl_mgr->GetContext();
	sl_mgr->CreateShader(sl::SPRITE2, new sl::Sprite2Shader(sl_rc));
	glue::RenderContext::Instance()->SetCamera(0, 0, 1, 1);

	s2::Symbol* sym = glue::SymbolFactory::Instance()->Create("coin_00.png");
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
	{
		s2::Sprite* spr = new s2::ImageSprite(sym);
		spr->SetPosition(sm::vec2(-100, 100));
		m_sprites.push_back(spr);
	}
	{
		s2::Sprite* spr = new s2::ImageSprite(sym);
		spr->SetPosition(sm::vec2(100, -100));
		m_sprites.push_back(spr);
	}
}

void TestSprite::Resize(int width, int height)
{
	glue::RenderContext::Instance()->OnSize(width, height);
}

void TestSprite::Draw() const
{
// 	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
// 		s2::Sprite* spr = m_sprites[i];
// 		sm::mat4 mt = spr->GetTransMatrix();
// 		spr->GetSymbol().Draw(mt);	
// 	}

	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* sl_shader = static_cast<sl::Sprite2Shader*>(sl_mgr->GetShader());
	sl_shader->SetColor(0xffff00ff, 0);
	{
		s2::RenderParams p;
		p.mt = m_sprites[0]->GetTransMatrix();
		s2::DrawNode::Draw(m_sprites[0], p);
	}
	sl_shader->SetColor(0xffffffff, 0);
	{
		s2::RenderParams p;
		p.mt = m_sprites[1]->GetTransMatrix();
		s2::DrawNode::Draw(m_sprites[1], p);
	}
	sl_shader->SetColor(0xffffff00, 0);
	{
		s2::RenderParams p;
		p.mt = m_sprites[2]->GetTransMatrix();
		s2::DrawNode::Draw(m_sprites[2], p);
	}
	sl_shader->SetColorMap(0x000000ff, 0x00002288, 0x00ff0000);
	{
		s2::RenderParams p;
		p.mt = m_sprites[3]->GetTransMatrix();
		s2::DrawNode::Draw(m_sprites[3], p);
	}
	sl_shader->SetColorMap(0x000000ff, 0x00880022, 0x00ff0000);
	{
		s2::RenderParams p;
		p.mt = m_sprites[4]->GetTransMatrix();
		s2::DrawNode::Draw(m_sprites[4], p);
	}
}

void TestSprite::Update()
{
}

}