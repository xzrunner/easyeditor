#include "TestSprite.h"

#include <rvg_render.h>

namespace test
{

void TestSprite::Init()
{
	glue::ShaderMgr::Instance()->Init();

	glue::Symbol* sym = new glue::Symbol("coin_00.png");
	m_spr = new glue::Sprite(sym);
}

void TestSprite::Resize(int width, int height)
{
	glue::ShaderMgr::Instance()->OnSize(width, height);
}

void TestSprite::Draw() const
{
	glue::mat4 mt = m_spr->GetTransMatrix();
	m_spr->GetSymbol().Draw(mt);
}

void TestSprite::Update()
{
}

}