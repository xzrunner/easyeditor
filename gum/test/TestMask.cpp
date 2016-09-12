#include "TestMask.h"

#include <shaderlab.h>
#include <sprite2/MaskSprite.h>
#include <sprite2/DrawNode.h>
#include <gum/RenderContext.h>
#include <gum/SymbolFactory.h>
#include <gum/SpriteFactory.h>
#include <gum/GUM_DTex.h>

namespace test
{

void TestMask::Init()
{
	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->CreateContext(4096);
	sl::RenderContext* sl_rc = sl_mgr->GetContext();
	sl_mgr->CreateShader(sl::SPRITE2, new sl::Sprite2Shader(sl_rc));
	sl_mgr->CreateShader(sl::MASK, new sl::MaskShader(sl_rc));
	gum::RenderContext::Instance()->SetCamera(0, 0, 1, 1);

	gum::DTex::Instance();

	s2::Symbol* sym = gum::SymbolFactory::Instance()->Create("raw\\test_mask.json");
	{
		s2::MaskSprite* spr = new s2::MaskSprite(sym);
		spr->SetPosition(sm::vec2(0, -100));
		m_sprites.push_back(spr);
	}
}

void TestMask::Resize(int width, int height)
{
	gum::RenderContext::Instance()->OnSize(width, height);
}

void TestMask::Draw() const
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

void TestMask::Update()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) 
	{
		s2::Sprite* spr = m_sprites[i];
		s2::RenderParams params;
		params.mt = spr->GetTransMatrix();
		params.color = spr->Color();
		spr->Update(params);
	}
}

}