#include "TestImage.h"

#include <rvg_render.h>

namespace test
{

void TestImage::Init()
{
	glue::ShaderMgr::Instance()->Init();

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

void TestImage::Resize(int width, int height)
{
	glue::ShaderMgr::Instance()->OnSize(width, height);
}

void TestImage::Draw() const
{
// 	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
// 		glue::Sprite* spr = m_sprites[i];
// 		glue::mat4 mt = spr->GetTransMatrix();
// 		spr->GetSymbol().Draw(mt);	
// 	}

	glue::ShaderMgr* mgr = glue::ShaderMgr::Instance();
	glue::SpriteShader* shader = static_cast<glue::SpriteShader*>(
		mgr->GetShader(glue::ShaderMgr::SPRITE));

	shader->SetColor(0xffff00ff, 0);
	{
		glue::Sprite* spr = m_sprites[0];
		glue::mat4 mt = spr->GetTransMatrix();
		spr->GetSymbol().Draw(mt);	
	}
	shader->SetColor(0xffffffff, 0);
	{
		glue::Sprite* spr = m_sprites[1];
		glue::mat4 mt = spr->GetTransMatrix();
		spr->GetSymbol().Draw(mt);	
	}
	shader->SetColor(0xffffff00, 0);
	{
		glue::Sprite* spr = m_sprites[2];
		glue::mat4 mt = spr->GetTransMatrix();
		spr->GetSymbol().Draw(mt);	
	}
	shader->SetMapColor(0x000000ff, 0x00002288, 0x00ff0000);
	{
		glue::Sprite* spr = m_sprites[3];
		glue::mat4 mt = spr->GetTransMatrix();
		spr->GetSymbol().Draw(mt);	
	}
	shader->SetMapColor(0x000000ff, 0x00880022, 0x00ff0000);
	{
		glue::Sprite* spr = m_sprites[4];
		glue::mat4 mt = spr->GetTransMatrix();
		spr->GetSymbol().Draw(mt);	
	}
}

void TestImage::Update()
{
}

}