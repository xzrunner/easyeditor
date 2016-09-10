#include "TestScale9.h"

#include <sprite2/Scale9Sprite.h>
#include <sprite2/DrawNode.h>
#include <gum/RenderContext.h>
#include <gum/SymbolFactory.h>
#include <gum/SpriteFactory.h>
#include <shaderlab.h>

namespace test
{

void TestScale9::Init()
{
	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->CreateContext(4096);
	sl::RenderContext* sl_rc = sl_mgr->GetContext();
	sl_mgr->CreateShader(sl::SPRITE2, new sl::Sprite2Shader(sl_rc));
	gum::RenderContext::Instance()->SetCamera(0, 0, 1, 1);

	s2::Symbol* sym = gum::SymbolFactory::Instance()->Create("scale9_scale9.json");
	{
		s2::Scale9Sprite* spr = new s2::Scale9Sprite(sym);
		spr->SetPosition(sm::vec2(0, 0));
		spr->Resize(200, 100);
		m_sprites.push_back(spr);
	}

	{
		s2::Sprite* spr = gum::SpriteFactory::Instance()->Create("coin_00.png");
		spr->SetPosition(sm::vec2(-150, 0));
		m_sprites.push_back(spr);
	}
}

void TestScale9::Resize(int width, int height)
{
	gum::RenderContext::Instance()->OnSize(width, height);
}

void TestScale9::Draw() const
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

void TestScale9::Update()
{
}

}