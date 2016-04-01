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

sm_mat4 Sprite::GetTransMatrix() const
{
	sm_mat4 mat;
	sm_mat4_transmat(&mat, m_pos.x, m_pos.y, 0);
	return mat;
}

}