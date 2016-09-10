#include "TestParticle3d.h"

#include <sprite2/Particle3dSprite.h>
#include <sprite2/DrawNode.h>
#include <gum/RenderContext.h>
#include <gum/SymbolFactory.h>
#include <gum/SpriteFactory.h>
#include <shaderlab.h>

namespace test
{

void TestParticle3d::Init()
{
	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->CreateContext(4096);
	sl::RenderContext* sl_rc = sl_mgr->GetContext();
	sl_mgr->CreateShader(sl::SPRITE2, new sl::Sprite2Shader(sl_rc));
	gum::RenderContext::Instance()->SetCamera(0, 0, 1, 1);

	s2::Symbol* sym = gum::SymbolFactory::Instance()->Create("test_particle.json");
	{
		s2::Particle3dSprite* spr = new s2::Particle3dSprite(sym);
		spr->SetPosition(sm::vec2(50, 0));
		spr->SetLoop(false);
		spr->Rotate(1);
		m_sprites.push_back(spr);
	}
}

void TestParticle3d::Resize(int width, int height)
{
	gum::RenderContext::Instance()->OnSize(width, height);
}

void TestParticle3d::Draw() const
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

void TestParticle3d::Update()
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