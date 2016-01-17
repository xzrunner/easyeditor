#include "TestSprite.h"

#include <rvg_render.h>
#include <sl_sprite.h>

namespace test
{

void TestSprite::Init()
{
	sl_sprite_init();
	sl_sprite_modelview(0, 0, 1, 1);

	glue::Symbol* sym = new glue::Symbol("coin_00.png");
	m_spr = new glue::Sprite(sym);
}

void TestSprite::Resize(int width, int height)
{
	sl_sprite_projection(width, height);
	sl_sprite_bind();
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