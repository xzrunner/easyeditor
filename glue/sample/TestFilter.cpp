#include "TestFilter.h"

#include <shaderlab.h>
#include <sprite2/ImageSprite.h>
#include <sprite2/DrawNode.h>
#include <glue/ImageSymbol.h>
#include <glue/RenderContext.h>

namespace test
{

void TestFilter::Init()
{
	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->CreateContext(4096);
	sl::RenderContext* sl_rc = sl_mgr->GetContext();
	sl_mgr->CreateShader(sl::FILTER, new sl::FilterShader(sl_rc));
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

void TestFilter::Resize(int width, int height)
{
	glue::RenderContext::Instance()->OnSize(width, height);
}

void TestFilter::Draw() const
{
// 	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
// 		s2::Sprite* spr = m_sprites[i];
// 		sm::mat4 mt = spr->GetTransMatrix();
// 		spr->GetSymbol().Draw(mt);	
// 	}

	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->SetShader(sl::FILTER);
	sl::FilterShader* sl_shader = static_cast<sl::FilterShader*>(sl_mgr->GetShader());
	sl_shader->SetMode(sl::FM_GAUSSIAN_BLUR_HORI);
	s2::DrawNode::Draw(m_sprites[0]);
	sl_shader->SetMode(sl::FM_HEAT_HAZE);
	s2::DrawNode::Draw(m_sprites[1]);
	sl_shader->SetMode(sl::FM_SHOCK_WAVE);
	s2::DrawNode::Draw(m_sprites[2]);
	sl_shader->SetMode(sl::FM_SWIRL);
	s2::DrawNode::Draw(m_sprites[3]);
	sl_shader->SetMode(sl::FM_BLUR);
	s2::DrawNode::Draw(m_sprites[4]);
}

void TestFilter::Update()
{
}

}