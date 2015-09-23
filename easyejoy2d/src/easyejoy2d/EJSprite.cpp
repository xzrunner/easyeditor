#include "EJSprite.h"
#include "EJScreen.h"

#include <dtex_package.h>
#include <dtex_shader.h>
#include <dtex_texture_pool.h>
#include <dtex_screen.h>

#include <stdlib.h>
#include <assert.h>

namespace eejoy2d
{

EJSprite::EJSprite(dtex_package* pkg, const char* name)
	: m_ej_pkg(pkg->ej_pkg)
	, m_ej_spr(NULL)
{
	LoadSprite(pkg, name);
}

EJSprite::EJSprite(ej_sprite* spr)
	: m_ej_pkg(NULL)
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
	srt.offx = x;
	srt.offy = y;
	srt.rot = 0;
	srt.scalex = srt.scaley = 1024;

	Draw(m_ej_spr, &srt, NULL);
}

void EJSprite::Translate(int dx, int dy)
{
	m_ej_srt.offx += dx;
	m_ej_srt.offy += dy;
}

void EJSprite::SetFrame(int frame)
{
	if (m_ej_spr) {
		ej_sprite_setframe(m_ej_spr, frame, false);
	}
}

void EJSprite::LoadSprite(dtex_package* pkg, const char* name)
{
	int id = dtex_get_spr_id(pkg, name);
	assert(id >= 0);

	int sz = ej_sprite_size(m_ej_pkg, id);
	if (sz == 0) {
		m_ej_spr = NULL;
		return;
	}

	m_ej_spr = (ej_sprite*)malloc(sz);
	ej_sprite_init(m_ej_spr, m_ej_pkg, id, sz);

	// todo component mount
}

void EJSprite::Draw(struct ej_sprite* spr, struct ej_srt* srt, struct ej_sprite_trans* ts)
{
	struct ej_sprite_trans temp;
	struct ej_matrix temp_matrix;
	struct ej_sprite_trans* t = ej_sprite_trans_mul(&spr->t, ts, &temp, &temp_matrix);

	switch (spr->type) {
	case TYPE_PICTURE:
		dtex_shader_program(PROGRAM_NORMAL);
		DrawQuad(spr->s.pic, srt, t);
		break;
	case TYPE_ANIMATION:
		DrawAnim(spr, srt, t);
		break;
	}
}

void EJSprite::DrawQuad(struct ej_pack_picture* picture, const struct ej_srt* srt, 
						const struct ej_sprite_trans* arg)
{
	struct matrix tmp;
	if (arg->mat == NULL) {
		ej_matrix_identity(&tmp);
	} else {
		tmp = *arg->mat;
	}
	ej_matrix_srt(&tmp, srt);
	int *m = tmp.m;
	float vb[16];
	for (int i = 0; i < picture->n; i++) {
		struct ej_pack_quad* q = &picture->rect[i];
		struct dtex_raw_tex* tex = dtex_pool_query(q->texid);
		dtex_shader_texture(tex->id);
		for (int j = 0; j < 4; j++) {
			int xx = q->screen_coord[j*2+0];
			int yy = q->screen_coord[j*2+1];
			float vx = (xx * m[0] + yy * m[2]) / 1024 + m[4];
			float vy = (xx * m[1] + yy * m[3]) / 1024 + m[5];
			dtex_screen_trans(&vx,&vy);

			float tx = q->texture_coord[j*2+0];
			float ty = q->texture_coord[j*2+1];

			vb[i*4+0] = vx;
			vb[i*4+1] = vy;
			vb[i*4+2] = tx;
			vb[i*4+3] = ty;
		}
		dtex_shader_draw(vb);
	}
}

void EJSprite::DrawAnim(struct ej_sprite* spr, struct ej_srt* srt, struct ej_sprite_trans* t)
{
	int frame = GetSpriteFrame(spr);
	if (frame < 0) {
		return;
	}

	struct ej_pack_animation *ani = spr->s.ani;
	struct ej_pack_frame* pf = &ani->frame[frame];
	for (int i = 0; i < pf->n; i++) {
		struct pack_part *pp = &pf->part[i];
		int index = pp->component_id;
		struct sprite * child = spr->data.children[index];
		if (child == NULL || (child->flags & SPRFLAG_INVISIBLE)) {
			continue;
		}
		struct sprite_trans temp2;
		struct matrix temp_matrix2;
		struct sprite_trans* ct = sprite_trans_mul(&pp->t, t, &temp2, &temp_matrix2);
		Draw(child, srt, ct);
	}
}

int EJSprite::GetSpriteFrame(struct ej_sprite* spr)
{
	if (spr->type != TYPE_ANIMATION) {
		return spr->start_frame;
	}
	if (spr->total_frame <= 0) {
		return -1;
	}
	int f = spr->frame % spr->total_frame;
	if (f < 0) {
		f += spr->total_frame;
	}
	return f + spr->start_frame;
}

}