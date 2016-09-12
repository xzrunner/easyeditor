#include "TestPack.h"

#include <sprite2/TrailSprite.h>
#include <sprite2/DrawNode.h>
#include <gum/RenderContext.h>
#include <gum/SymbolFactory.h>
#include <gum/SpriteFactory.h>
#include <shaderlab.h>

namespace test
{

void TestPack::Init()
{
	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->CreateContext(4096);
	sl::RenderContext* sl_rc = sl_mgr->GetContext();
	sl_mgr->CreateShader(sl::SHAPE2, new sl::Shape2Shader(sl_rc));
	sl_mgr->CreateShader(sl::SPRITE2, new sl::Sprite2Shader(sl_rc));
	gum::RenderContext::Instance()->SetCamera(0, 0, 1, 1);

	s2::Symbol* sym = gum::SymbolFactory::Instance()->Create("raw\\shape_trail.json");
	{
		s2::TrailSprite* spr = new s2::TrailSprite(sym);
		spr->SetPosition(sm::vec2(50, 0));
		m_sprites.push_back(spr);
	}
}

void TestPack::Resize(int width, int height)
{
	gum::RenderContext::Instance()->OnSize(width, height);
}

void TestPack::Draw() const
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

void TestPack::Update()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) 
	{
		s2::Sprite* spr = m_sprites[i];

		spr->Translate(sm::vec2(0.1, 0.1));

		s2::RenderParams params;
		params.mt = spr->GetTransMatrix();
		params.color = spr->Color();
		spr->Update(params);
	}
}

}