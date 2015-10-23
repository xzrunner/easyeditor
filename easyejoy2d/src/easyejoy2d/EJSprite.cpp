#include "EJSprite.h"
#include "EJScreen.h"

#include <dtex.h>

#include <stdlib.h>
#include <assert.h>

namespace eejoy2d
{

EJSprite::EJSprite(dtex_package* pkg, const char* name)
	: m_pkg(pkg)
	, m_ej_spr(NULL)
{
	int id = dtex_get_spr_id(pkg, name);
	assert(id >= 0);
//	m_ej_spr = dtex_ej_sprite_create(pkg, id);
}

EJSprite::EJSprite(ej_sprite* spr)
	: m_pkg(NULL)
	, m_ej_spr(spr)
{
}

EJSprite::~EJSprite()
{
//	free(m_spr);	
}

void EJSprite::Update()
{
	if (m_ej_spr) {
		SetFrame(m_ej_spr->frame + 1);
	}
}

void EJSprite::Draw(int x, int y) const
{
	struct ej_srt srt;
	srt.offx = x * 16;
	srt.offy = y * 16;
	srt.rot = 0;
	srt.scalex = srt.scaley = 1024;

//	Draw(m_ej_spr, &srt, NULL);

	//dtexf_sprite_draw(m_pkg, m_ej_spr, &srt);
}

// void EJSprite::Translate(int dx, int dy)
// {
// 	m_ej_srt.offx += dx;
// 	m_ej_srt.offy += dy;
// }

void EJSprite::SetFrame(int frame)
{
	if (m_ej_spr) {
		ej_sprite_setframe(m_ej_spr, frame, false);
	}
}

}