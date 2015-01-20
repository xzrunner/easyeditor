#include "EJSprite.h"
#include "EJScreen.h"

#include <sprite.h>
#include <shader.h>
#include <package.h>
#include <ejoy2d.h>

#include <stdlib.h>
#include <assert.h>

namespace eejoy2d
{

EJSprite::EJSprite(ej_package* pkg, const char* name)
	: m_pkg(pkg)
	, m_spr(NULL)
{
	LoadSprite(pkg, name);
}

EJSprite::EJSprite(ej_sprite* spr)
	: m_pkg(NULL)
	, m_spr(spr)
{
}

EJSprite::~EJSprite()
{
	free(m_spr);	
}

void EJSprite::Update()
{
	if (m_spr) {
		++m_spr->frame;
	}
}

void EJSprite::Draw(int x, int y) const
{
	if (!m_spr || m_spr->pack == NULL || m_spr->pack->texture_n == 0) {
		return;
	}

	if (m_spr->flag & EJ_ADDITIVEMODE) {
		shader_blend(GL_ONE);
	} else {
		shader_blend(GL_ONE_MINUS_SRC_ALPHA);
	}
	int* mat = NULL;
	if (m_spr->flag & EJ_MATRIX) {
		mat = m_spr->mat;
	}

	screen_coord screen;
	static ej_extra def_extra;
	def_extra.offx = def_extra.offy = 0;
	def_extra.scale = 1;
	def_extra.ud = def_extra.extra = NULL;
	struct ej_extra* extra = &def_extra;
	sprite_default_coord(&screen);

	x *= SCALE;
	y *= SCALE;

	Draw(m_spr, NULL, 0, x, y, m_spr->color_trans, m_spr->color_additive, mat, extra, &screen);
}

void EJSprite::LoadSprite(ej_package* pkg, const char* name)
{
	int id = sprite_id(pkg, name);
	assert(id >= 0);
	int sz = sprite_size(pkg, id);

	m_spr = (ej_sprite*)malloc(sz);
	int ret = sprite_create(m_spr, pkg, id);
	assert(ret == sz);
}

static inline unsigned int
clamp(unsigned int c) {
	return ((c) > 255 ? 255 : (c));
}

static inline unsigned int
color_add(unsigned int c1, unsigned int c2) {
	if (c1 == 0)
		return c2;
	if (c2 == 0)
		return c1;
	int r1 = (c1 >> 16) & 0xff;
	int g1 = (c1 >> 8) & 0xff;
	int b1 = (c1) & 0xff;
	int r2 = (c2 >> 16) & 0xff;
	int g2 = (c2 >> 8) & 0xff;
	int b2 = (c2) & 0xff;
	return clamp(r1+r2) << 16 |
		clamp(g1+g2) << 8 |
		clamp(b1+b2);
}

static inline unsigned int
color_mul(unsigned int c1, unsigned int c2) {
	if (c1 == 0xffffffff)
		return c2;
	if (c2 == 0xffffffff)
		return c1;
	int r1 = (c1 >> 24) & 0xff;
	int g1 = (c1 >> 16) & 0xff;
	int b1 = (c1 >> 8) & 0xff;
	int a1 = (c1) & 0xff;
	int r2 = (c2 >> 24) & 0xff;
	int g2 = (c2 >> 16) & 0xff;
	int b2 = (c2 >> 8) & 0xff;
	int a2 = c2 & 0xff;

	return (r1 * r2 /255) << 24 |
		(g1 * g2 /255) << 16 |
		(b1 * b2 /255) << 8 |
		(a1 * a2 /255) ;
}

static int *
matrix_combine(int *m1, int *m2, int *tmp) {
	if (m1 == NULL)
		return m2;
	if (m2 == NULL)
		return m1;
	matrix_mul(tmp,m1,m2);
	return tmp;
}

void EJSprite::Draw(const ej_sprite* spr, animation* opt_ani, int frame, int x, int y, 
					uint32_t color, uint32_t additive, int *mat, ej_extra *extra, screen_coord * screen) const
{
	animation* ani;
	int action = 0;
	if (opt_ani == NULL) {
		if (spr->flag & EJ_INVISIBLE) {
			return;
		}
		ani = spr->ani;
		frame = spr->frame;
		action = spr->action;
	} else {
		ani = opt_ani;
	}
	if (ani == NULL || ani->part_n == 0) {
		return;
	}
	
	// draw picture
	if (ani->part_n < 0) {
		int shader = (spr->flag & EJ_GRAY) ? PROGRAM_GRAY : PROGRAM_SPRITE;
		sprite_draw(spr->pack, (struct picture *)ani, x , y, mat, color, additive, (spr->flag & EJ_MIRROR), shader, screen);
		return;
	}

	struct animation_action * act = &ani->action[action];
	if (act->n == 0)
		return;

	int f = ((unsigned int)frame) % act->n;
	struct animation_frame *af = &act->frame[f];
	struct ejoypic * ep = spr->pack->ep;
	int i;
	for (i=0;i<af->n;i++) 
	{
		struct animation_component *c = &af->c[i];
		if (c->flag) {
			screen_clip_push(mat, x,y , ani->clipbox, extra->scale,0);
		}

		struct animation_part * part = &ani->part[c->index];
		int m[6];
		int tmp[6];
		int *rm;
		struct label * text = part->text;
		uint32_t tcolor = color_mul(color, c->color);
		uint32_t tadditive = color_add(additive, c->additive);

		if (opt_ani == NULL && part->name) {
			int *rm;
			struct ej_sprite * subs = spr->c[c->index];
			//      mount can change sub sprite
			//      assert(subs->ani == ep->spr[part->id]);
			if (subs->flag & EJ_MATRIX) {
				rm = matrix_combine(subs->mat, c->mat, tmp);
				rm = matrix_combine(rm, mat, m);
			} else {
				rm = matrix_combine(c->mat, mat, m);
			}

			tcolor = color_mul(tcolor, subs->color_trans);
			tadditive = color_add(tadditive, subs->color_additive);

			Draw(subs,NULL,0,x,y,tcolor, tadditive, rm,extra,screen);
		} else {
			if (part->id < 0 || part->id >= ep->max_id) {
				goto _continue;
			}
			struct animation * subani = ep->spr[part->id];
			if (subani) {
				int *rm = matrix_combine(c->mat, mat, m);
				Draw(spr, subani, frame, x,y,tcolor,tadditive,rm,extra,screen);
			}
		}
_continue:
		if (c->flag) {
			screen_clip_pop(0);
		}
	}
}

void EJSprite::Translate(int dx, int dy)
{
	if (m_spr) {
		m_spr->mat[4] += dx * 16;
		m_spr->mat[5] += dy * 16;
	}
}

}