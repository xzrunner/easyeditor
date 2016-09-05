#include "TestSprite.h"

#include <sprite2/ImageSprite.h>
#include <sprite2/DrawNode.h>
#include <glue/RenderContext.h>
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
		spr->Color().mul = 0xffff00ff;
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
		spr->Color().mul = 0x00ffffff;
		m_sprites.push_back(spr);
	}
	{
		s2::Sprite* spr = new s2::ImageSprite(sym);
		spr->SetPosition(sm::vec2(-100, 100));
		spr->Color().gmap = 0x00002288;
		m_sprites.push_back(spr);
	}
	{
		s2::Sprite* spr = new s2::ImageSprite(sym);
		spr->SetPosition(sm::vec2(100, -100));
		spr->Color().gmap = 0x00880022;
		m_sprites.push_back(spr);
	}
}

void TestSprite::Resize(int width, int height)
{
	glue::RenderContext::Instance()->OnSize(width, height);
}

void TestSprite::Draw() const
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) 
	{
		s2::Sprite* spr = m_sprites[i];
		s2::RenderParams params;
		params.mt = spr->GetTransMatrix();
		params.color = spr->Color();
		s2::DrawNode::Draw(spr, params);
	}
}

void TestSprite::Update()
{
}

}