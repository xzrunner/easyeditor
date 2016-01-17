#include "Symbol.h"
#include "Texture.h"

#include "math/Math.h"

#include <sl_sprite.h>

namespace glue
{

Symbol::Symbol(const std::string& filepath)
	: m_filepath(filepath)
	, m_tex(NULL)
{
	Load();
}

void Symbol::Draw(const mat4& mt) const
{
	RID texid = m_tex->GetID();

	vec2 positions[4];
	float hw = m_tex->GetWidth() * 0.5f,
		  hh = m_tex->GetHeight() * 0.5f;
	positions[0] = vec2(-hw, -hh);
	positions[1] = vec2( hw, -hh);
	positions[2] = vec2( hw,  hh);
	positions[3] = vec2(-hw,  hh);
	for (int i = 0; i < 4; ++i) {
		positions[i] = Math::TransVector(positions[i], mt);
	}

	vec2 texcoords[4];
	texcoords[0] = vec2(0, 0);
	texcoords[1] = vec2(1, 0);
	texcoords[2] = vec2(1, 1);
	texcoords[3] = vec2(0, 1);

	sl_sprite_bind();
	sl_sprite_draw(&positions[0].x, &texcoords[0].x, texid);
}

void Symbol::Load()
{
	m_tex = new glue::Texture();
	m_tex->Load("coin_00.png");
}

}