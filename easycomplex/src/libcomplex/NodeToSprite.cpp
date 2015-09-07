#include "NodeToSprite.h"
#include "Symbol.h"
#include "Sprite.h"

#include <easyicon.h>
#include <easyanim.h>

namespace ecomplex
{

d2d::ISprite* NodeToSprite::ToSprite(const libcoco::IPackNode* node)
{
	if (const libcoco::PackPicture* pic = dynamic_cast<const libcoco::PackPicture*>(node)) {
		return Pic2Sprite(pic);
	} else if (const libcoco::PackLabel* label = dynamic_cast<const libcoco::PackLabel*>(node)) {
		return Label2Sprite(label);
	} else if (const libcoco::PackAnimation* anim = dynamic_cast<const libcoco::PackAnimation*>(node)) {
		return Anim2Sprite(anim);
	} else {
		throw d2d::Exception("NodeToSprite::Node2Sprite unknown type.");
	}
}

d2d::ISprite* NodeToSprite::Quad2Sprite(const libcoco::PackPicture::Quad* quad)
{
	float w = quad->img->GetOriginWidth(),
		h = quad->img->GetOriginHeight();

	d2d::Vector src[4], screen[4];
	for (int i = 0; i < 4; ++i) {
		src[i].x = quad->texture_coord[i].x / w;
		src[i].y = 1 - quad->texture_coord[i].y / h;
		screen[i] = quad->screen_coord[i];
	}

	eicon::QuadIcon* icon = new eicon::QuadIcon(
		const_cast<d2d::Image*>(quad->img), src, screen);

	eicon::Symbol* symbol = new eicon::Symbol;
	symbol->SetIcon(icon);
	d2d::ISprite* ret = new eicon::Sprite(symbol);
	ret->BuildBounding();
	return ret;
}

d2d::ISprite* NodeToSprite::Pic2Sprite(const libcoco::PackPicture* pic)
{
	if (pic->quads.size() == 1) {
		return Quad2Sprite(&pic->quads[0]);
	} else if (pic->quads.size() > 1) {
		ecomplex::Symbol* complex = new ecomplex::Symbol;
		for (int i = 0, n = pic->quads.size(); i < n; ++i) {
			complex->m_sprites.push_back(Quad2Sprite(&pic->quads[i]));
		}
		complex->InitBounding();
		return new Sprite(complex);
	} else {
		return NULL;
	}
}

d2d::ISprite* NodeToSprite::Label2Sprite(const libcoco::PackLabel* label)
{
	d2d::FontBlankSymbol* symbol = new d2d::FontBlankSymbol();

	d2d::FontSprite* spr = new d2d::FontSprite(symbol);
	spr->font = label->font;
	spr->has_edge = label->has_edge;
	spr->color = label->color;
	spr->align_hori = label->align_hori;
	spr->align_vert = label->align_vert;
	spr->size = label->size;
	spr->width = label->width;
	spr->height = label->height;
	return spr;
}

d2d::ISprite* NodeToSprite::Anim2Sprite(const libcoco::PackAnimation* anim)
{
	assert(!anim->actions.empty() && anim->actions[0].size >= 1);
	if (anim->actions[0].size == 1) {
		return Anim2ComplexSprite(anim);
	} else {
		return Anim2AnimSprite(anim);
	}
}

d2d::ISprite* NodeToSprite::Anim2ComplexSprite(const libcoco::PackAnimation* anim)
{
	assert(!anim->actions.empty() && anim->actions[0].size == 1);

	ecomplex::Symbol* complex = new ecomplex::Symbol;
	const libcoco::PackAnimation::Frame& src = anim->frames[0];
	ecomplex::Symbol* dst = new ecomplex::Symbol;
	for (int i = 0; i < src.parts.size(); ++i) {
		const libcoco::PackAnimation::Part& part = src.parts[i];
		d2d::ISprite* spr = ToSprite(anim->components[part.comp_idx].node);
		TransSprite(spr, part.t);
		dst->m_sprites.push_back(spr);
	}
	dst->InitBounding();
	complex->m_sprites.push_back(new Sprite(dst));
	complex->InitBounding();
	return new Sprite(complex);
}

d2d::ISprite* NodeToSprite::Anim2AnimSprite(const libcoco::PackAnimation* anim)
{
	assert(!anim->actions.empty() && anim->actions[0].size >= 1);

	libanim::Symbol* anim_symbol = new libanim::Symbol;
	libanim::Symbol::Layer* layer = new libanim::Symbol::Layer;
	for (int i = 0; i < anim->actions[0].size; ++i) {
		const libcoco::PackAnimation::Frame& src = anim->frames[i];
		libanim::Symbol::Frame* frame = new libanim::Symbol::Frame;
		frame->index = i;
		frame->bClassicTween = false;
		for (int j = 0; j < src.parts.size(); ++j) {
			const libcoco::PackAnimation::Part& part = src.parts[j];
			d2d::ISprite* spr = ToSprite(anim->components[part.comp_idx].node);
			TransSprite(spr, part.t);
			frame->sprites.push_back(spr);
		}
		layer->frames.push_back(frame);
	}
	anim_symbol->setFPS(30);
	anim_symbol->m_layers.push_back(layer);
	anim_symbol->InitBounding();
	return new libanim::Sprite(anim_symbol);
}

void NodeToSprite::TransSprite(d2d::ISprite* spr, const libcoco::PackAnimation::SpriteTrans& t)
{
	if (!libcoco::PackAnimation::IsMatrixIdentity(t.mat)) {
		TransSpriteMat(spr, t);
	}
	TransSpriteCol(spr, t);
}

void NodeToSprite::TransSpriteMat(d2d::ISprite* spr, const libcoco::PackAnimation::SpriteTrans& t)
{
	float dx = t.mat[4] / 16.0f,
		dy = -t.mat[5] / 16.0f;

	// no shear
	// 	mat[0] = sx*c;
	// 	mat[1] = sx*s;
	// 	mat[2] = -sy*s;
	// 	mat[3] = sy*c;	

	float angle = atan2((float)t.mat[1], (float)t.mat[0]);
	// #ifdef _DEBUG
	// 	if (t.mat[2] != 0) {
	// 		assert(atan2(-(float)t.mat[2], (float)t.mat[3]) == angle);
	// 	}
	// #endif
	float c = cos(angle), s = sin(angle);
	float sx, sy;
	if (c != 0) {
		sx = t.mat[0] / c / 1024.0f;
		sy = t.mat[3] / c / 1024.0f;
	} else {
		sx = t.mat[1] / s / 1024.0f;
		sy = -t.mat[2] / s / 1024.0f;
	}

	if (fabs(t.mat[0] - sx * c * 1024) >= 1 ||
		fabs(t.mat[1] - sx * s * 1024) >= 1 ||
		fabs(t.mat[2] - -sy * s * 1024) >= 1 ||
		fabs(t.mat[3] - sy * c * 1024) >= 1) {
			// todo use mat for sprite

			sx = sy = 1;
			angle = 0;
	}

	// no scale
	// 	mat[0] = c - ky*s;
	// 	mat[1] = s + ky*c;
	// 	mat[2] = kx*c - s;
	// 	mat[3] = kx*s + c;

	bool xmirror = false, ymirror = false;
	if (sx < 0) {
		xmirror = true;
		sx = -sx;
	}
	if (sy < 0) {
		ymirror = true;
		sy = -sy;
	}
	spr->SetMirror(xmirror, ymirror);

	spr->SetScale(sx, sy);
	spr->SetTransform(d2d::Vector(dx, dy), angle);
}

void NodeToSprite::TransSpriteCol(d2d::ISprite* spr, const libcoco::PackAnimation::SpriteTrans& t)
{
	spr->multiCol = d2d::transColor(t.color, d2d::PT_ARGB);
	spr->addCol = d2d::transColor(t.additive, d2d::PT_ARGB);

	spr->r_trans = d2d::transColor(t.rmap, d2d::PT_ARGB);
	spr->g_trans = d2d::transColor(t.gmap, d2d::PT_ARGB);
	spr->b_trans = d2d::transColor(t.bmap, d2d::PT_ARGB);
}

}