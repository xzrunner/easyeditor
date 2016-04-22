#include "TestFilter.h"

#include <shaderlab.h>

namespace test
{

void TestFilter::Init()
{
	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->CreateContext(4096);
	sl::RenderContext* sl_rc = sl_mgr->GetContext();
	sl_mgr->CreateShader(sl::FILTER, new sl::FilterShader(sl_rc));
	glue::RenderContext::Instance()->SetCamera(0, 0, 1, 1);

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
	{
		glue::Sprite* spr = new glue::Sprite(sym);
		spr->SetPositioin(glue::vec2(-100, 100));
		m_sprites.push_back(spr);
	}
	{
		glue::Sprite* spr = new glue::Sprite(sym);
		spr->SetPositioin(glue::vec2(100, -100));
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
// 		glue::Sprite* spr = m_sprites[i];
// 		sm_mat4 mt = spr->GetTransMatrix();
// 		spr->GetSymbol().Draw(mt);	
// 	}

	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->SetShader(sl::FILTER);
	sl::FilterShader* sl_shader = static_cast<sl::FilterShader*>(sl_mgr->GetShader());
	sl_shader->SetMode(sl::FM_GAUSSIAN_BLUR);
	{
		glue::Sprite* spr = m_sprites[0];
		sm_mat4 mt = spr->GetTransMatrix();
		spr->GetSymbol().Draw(mt);	
	}
	sl_shader->SetMode(sl::FM_HEAT_HAZE);
	{
		glue::Sprite* spr = m_sprites[1];
		sm_mat4 mt = spr->GetTransMatrix();
		spr->GetSymbol().Draw(mt);	
	}
	sl_shader->SetMode(sl::FM_SHOCK_WAVE);
	{
		glue::Sprite* spr = m_sprites[2];
		sm_mat4 mt = spr->GetTransMatrix();
		spr->GetSymbol().Draw(mt);	
	}
	sl_shader->SetMode(sl::FM_SWIRL);
	{
		glue::Sprite* spr = m_sprites[3];
		sm_mat4 mt = spr->GetTransMatrix();
		spr->GetSymbol().Draw(mt);	
	}
	sl_shader->SetMode(sl::FM_BLUR);
	{
		glue::Sprite* spr = m_sprites[4];
		sm_mat4 mt = spr->GetTransMatrix();
		spr->GetSymbol().Draw(mt);	
	}
}

void TestFilter::Update()
{
}

}