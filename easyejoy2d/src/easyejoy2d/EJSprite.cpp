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
	LoadSprite(pkg, name);
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
	srt.offx = x * 16 - 5000;
	srt.offy = y * 16 - 5000;
	srt.rot = 0;
	srt.scalex = srt.scaley = 128;

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
	m_ej_spr = CreateSprite(id);
}

ej_sprite* EJSprite::CreateSprite(int id)
{
	int sz = ej_sprite_size(m_pkg->ej_pkg, id);
	if (sz == 0) {
		m_ej_spr = NULL;
		return NULL;
	}

	ej_sprite* spr = (ej_sprite*)malloc(sz);
	ej_sprite_init(spr, m_pkg->ej_pkg, id, sz);

	for (int i = 0; ; ++i) {
		int childid = ej_sprite_component(spr, i);
		if (childid < 0) {
			break;
		}

		struct sprite* c = CreateSprite(childid);
		if (c) {
			c->name = ej_sprite_childname(spr, i);
			sprite_mount(spr, i, c);
// 			if (spr->type == TYPE_ANIMATION) {
// 				update_anim_message(c, pack, id, i, spr->frame);
// 			} else if (spr->type == TYPE_PARTICLE3D) {
// 				update_p3d_message(c, pack, id, i);
// 			} else if (spr->type == TYPE_PARTICLE2D) {
// 				update_p2d_message(c, pack, id, i);
// 			}
		}
	}

	return spr;
}

void EJSprite::Draw(struct ej_sprite* spr, struct ej_srt* srt, struct ej_sprite_trans* ts) const
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
						const struct ej_sprite_trans* arg) const
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

		struct dtex_raw_tex* tex = NULL;
		if (q->texid < QUAD_TEXID_IN_PKG_MAX) {
			assert(q->texid < m_pkg->tex_size);
			tex = m_pkg->textures[q->texid];
		} else {
			tex = dtex_pool_query(q->texid - QUAD_TEXID_IN_PKG_MAX);
		}

		for (int j = 0; j < 4; j++) {
			int xx = q->screen_coord[j*2+0];
			int yy = q->screen_coord[j*2+1];
			float vx = (xx * m[0] + yy * m[2]) / 1024 + m[4];
			float vy = (xx * m[1] + yy * m[3]) / 1024 + m[5];
			dtex_screen_trans(&vx,&vy);

			float tx = q->texture_coord[j*2+0];
			float ty = q->texture_coord[j*2+1];
			tx /= tex->width;
			ty /= tex->height;

			vb[j*4+0] = vx;
			vb[j*4+1] = vy;
			vb[j*4+2] = tx;
			vb[j*4+3] = ty;
		}

		int texid = tex->id;

		int new_texid = 0;
		float* tex_vb = dtexf_c2_lookup_texcoords(tex, vb, &new_texid);
		if (tex_vb != NULL) {
			memcpy(vb+2, tex_vb, 2*sizeof(float));
			memcpy(vb+6, tex_vb+2, 2*sizeof(float));
			memcpy(vb+10, tex_vb+4, 2*sizeof(float));
			memcpy(vb+14, tex_vb+6, 2*sizeof(float));  
			texid = new_texid;

			dtex_shader_texture(texid);
			dtex_shader_draw(vb);
		}

	}
}

void EJSprite::DrawAnim(struct ej_sprite* spr, struct ej_srt* srt, struct ej_sprite_trans* t) const
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