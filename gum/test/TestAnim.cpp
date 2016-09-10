#include "TestAnim.h"

#include <sprite2/AnimSprite.h>
#include <sprite2/DrawNode.h>
#include <gum/RenderContext.h>
#include <gum/SymbolFactory.h>
#include <gum/SpriteFactory.h>
#include <gum/GUM_GTxt.h>
#include <shaderlab.h>

namespace test
{

void TestAnim::Init()
{
	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->CreateContext(4096);
	sl::RenderContext* sl_rc = sl_mgr->GetContext();
	sl_mgr->CreateShader(sl::SHAPE2, new sl::Shape2Shader(sl_rc));
	sl_mgr->CreateShader(sl::SPRITE2, new sl::Sprite2Shader(sl_rc));
	gum::RenderContext::Instance()->SetCamera(0, 0, 1, 1);

//	s2::Symbol* sym = gum::SymbolFactory::Instance()->Create("test_anim.json");
	s2::Symbol* sym = gum::SymbolFactory::Instance()->Create("trail_anim.json");
	{
		s2::AnimSprite* spr = new s2::AnimSprite(sym);
		spr->SetPosition(sm::vec2(0, 0));
		m_sprites.push_back(spr);
	}
}

void TestAnim::Resize(int width, int height)
{
	gum::RenderContext::Instance()->OnSize(width, height);

// 	static bool inited = false;
// 	if (!inited) {
// 		InitGTxt();
// 	}
}

void TestAnim::Draw() const
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

void TestAnim::Update()
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