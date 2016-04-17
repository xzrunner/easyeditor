#include "TestSprite.h"

#include <rvg_render.h>
#include <shaderlab.h>

namespace test
{

void TestSprite::Init()
{
	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->CreateContext(4096);
	sl::RenderContext* sl_rc = sl_mgr->GetContext();
	sl_mgr->CreateShader(sl::SPRITE, new sl::SpriteShader(sl_rc));
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

void TestSprite::Resize(int width, int height)
{
	glue::RenderContext::Instance()->OnSize(width, height);
}

void TestSprite::Draw() const
{
// 	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
// 		glue::Sprite* spr = m_sprites[i];
// 		sm_mat4 mt = spr->GetTransMatrix();
// 		spr->GetSymbol().Draw(mt);	
// 	}

	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->SetShader(sl::SPRITE);
	sl::SpriteShader* sl_shader = static_cast<sl::SpriteShader*>(sl_mgr->GetShader());
	sl_shader->SetColor(0xffff00ff, 0);
	{
		glue::Sprite* spr = m_sprites[0];
		sm_mat4 mt = spr->GetTransMatrix();
		spr->GetSymbol().Draw(mt);	
	}
	sl_shader->SetColor(0xffffffff, 0);
	{
		glue::Sprite* spr = m_sprites[1];
		sm_mat4 mt = spr->GetTransMatrix();
		spr->GetSymbol().Draw(mt);	
	}
	sl_shader->SetColor(0xffffff00, 0);
	{
		glue::Sprite* spr = m_sprites[2];
		sm_mat4 mt = spr->GetTransMatrix();
		spr->GetSymbol().Draw(mt);	
	}
	sl_shader->SetColorMap(0x000000ff, 0x00002288, 0x00ff0000);
	{
		glue::Sprite* spr = m_sprites[3];
		sm_mat4 mt = spr->GetTransMatrix();
		spr->GetSymbol().Draw(mt);	
	}
	sl_shader->SetColorMap(0x000000ff, 0x00880022, 0x00ff0000);
	{
		glue::Sprite* spr = m_sprites[4];
		sm_mat4 mt = spr->GetTransMatrix();
		spr->GetSymbol().Draw(mt);	
	}
}

void TestSprite::Update()
{
}

}