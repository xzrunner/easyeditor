#include "Sprite.h"

namespace glue
{

Sprite::Sprite(Symbol* sym)
	: m_sym(sym)
{
}

Sprite::~Sprite()
{
	// todo
}

void Sprite::Update()
{
}

mat4 Sprite::GetTransMatrix() const
{
	return mat4::Translate(m_pos.x, m_pos.y, 0);
}

}