#include "AnimationToSpr.h"
#include "NodeToSprite.h"
#include "Symbol.h"
#include "Sprite.h"

#include <ee/trans_color.h>

#include <easyanim.h>

namespace ecomplex
{

ee::Sprite* AnimationToSpr::Trans(const erespacker::PackAnimation* anim)
{
	assert(!anim->actions.empty() && anim->actions[0].size >= 1);
	if (anim->actions[0].size == 1) {
		return TransComplex(anim);
	} else {
		return TransAnim(anim);
	}
}

ee::Sprite* AnimationToSpr::TransComplex(const erespacker::PackAnimation* anim)
{
	assert(!anim->actions.empty() && anim->actions[0].size == 1);

	ecomplex::Symbol* complex = new ecomplex::Symbol;
	const erespacker::PackAnimation::Frame& src = anim->frames[0];
	ecomplex::Symbol* dst = new ecomplex::Symbol;
	for (int i = 0, n = src.parts.size(); i < n; ++i) {
		const erespacker::PackAnimation::Part& part = src.parts[i];
		ee::Sprite* spr = NodeToSprite::Trans(anim->components[part.comp_idx].node);
		TransSprite(spr, part.t);
		dst->m_sprites.push_back(spr);
	}
	dst->InitBounding();
	complex->m_sprites.push_back(new Sprite(dst));
	complex->InitBounding();
	return new Sprite(complex);
}

ee::Sprite* AnimationToSpr::TransAnim(const erespacker::PackAnimation* anim)
{
	assert(!anim->actions.empty() && anim->actions[0].size >= 1);

	eanim::Symbol* anim_symbol = new eanim::Symbol;
	eanim::Symbol::Layer* layer = new eanim::Symbol::Layer;
	for (int i = 0; i < anim->actions[0].size; ++i) {
		const erespacker::PackAnimation::Frame& src = anim->frames[i];
		eanim::Symbol::Frame* frame = new eanim::Symbol::Frame;
		frame->index = i;
		frame->bClassicTween = false;
		for (int j = 0, m = src.parts.size(); j < m; ++j) {
			const erespacker::PackAnimation::Part& part = src.parts[j];
			ee::Sprite* spr = NodeToSprite::Trans(anim->components[part.comp_idx].node);
			TransSprite(spr, part.t);
			frame->sprites.push_back(spr);
		}
		layer->frames.push_back(frame);
	}
	anim_symbol->setFPS(30);
	anim_symbol->m_layers.push_back(layer);
	anim_symbol->InitBounding();
	return new eanim::Sprite(anim_symbol);
}

void AnimationToSpr::TransSprite(ee::Sprite* spr, const erespacker::PackAnimation::SpriteTrans& t)
{
	if (!erespacker::PackAnimation::IsMatrixIdentity(t.mat)) {
		TransSpriteMat(spr, t);
	}
	TransSpriteCol(spr, t);
	spr->rp->shader.blend = (ee::BlendModes::Instance()->GetIDFromIdx(t.blend));
}

void AnimationToSpr::TransSpriteMat(ee::Sprite* spr, const erespacker::PackAnimation::SpriteTrans& t)
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

	spr->SetScale(sm::vec2(sx, sy));
	spr->SetTransform(sm::vec2(dx, dy), angle);
}

void AnimationToSpr::TransSpriteCol(ee::Sprite* spr, const erespacker::PackAnimation::SpriteTrans& t)
{
	spr->rp->color.multi = ee::TransColor(t.color, ee::PT_ARGB);
	spr->rp->color.add = ee::TransColor(t.additive, ee::PT_ARGB);

	spr->rp->color.r = ee::TransColor(t.rmap, ee::PT_RGBA);
	spr->rp->color.g = ee::TransColor(t.gmap, ee::PT_RGBA);
	spr->rp->color.b = ee::TransColor(t.bmap, ee::PT_RGBA);
}


}