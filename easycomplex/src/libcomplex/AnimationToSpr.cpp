#include "AnimationToSpr.h"
#include "NodeToSprite.h"
#include "Symbol.h"
#include "Sprite.h"

#include <ee/BlendModes.h>

#include <glue/trans_color.h>

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
		dst->Add(spr);
	}
//	dst->InitBounding();
	complex->Add(new Sprite(dst));
//	complex->InitBounding();
	return new Sprite(complex);
}

ee::Sprite* AnimationToSpr::TransAnim(const erespacker::PackAnimation* anim)
{
	assert(!anim->actions.empty() && anim->actions[0].size >= 1);

	eanim::Symbol* anim_symbol = new eanim::Symbol;
	s2::AnimSymbol::Layer* layer = new s2::AnimSymbol::Layer;
	for (int i = 0; i < anim->actions[0].size; ++i) {
		const erespacker::PackAnimation::Frame& src = anim->frames[i];
		s2::AnimSymbol::Frame* frame = new s2::AnimSymbol::Frame;
		frame->index = i;
		frame->tween = false;
		for (int j = 0, m = src.parts.size(); j < m; ++j) {
			const erespacker::PackAnimation::Part& part = src.parts[j];
			ee::Sprite* spr = NodeToSprite::Trans(anim->components[part.comp_idx].node);
			TransSprite(spr, part.t);
			frame->sprs.push_back(spr);
		}
		layer->frames.push_back(frame);
	}
	anim_symbol->SetFPS(30);
	anim_symbol->AddLayer(layer);
//	anim_symbol->InitBounding();
	return new eanim::Sprite(anim_symbol);
}

void AnimationToSpr::TransSprite(ee::Sprite* spr, const erespacker::PackAnimation::SpriteTrans& t)
{
	if (!erespacker::PackAnimation::IsMatrixIdentity(t.mat)) {
		TransSpriteMat(spr, t);
	}
	TransSpriteCol(spr, t);
	spr->Shader().blend = (ee::BlendModes::Instance()->ID2Mode(t.blend));
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

	spr->SetScale(sm::vec2(sx, sy));
	spr->SetPosition(sm::vec2(dx, dy));
	spr->SetAngle(angle);
}

void AnimationToSpr::TransSpriteCol(ee::Sprite* spr, const erespacker::PackAnimation::SpriteTrans& t)
{
	spr->Color().mul = int2color(t.color, glue::PT_ARGB);
	spr->Color().add = int2color(t.additive, glue::PT_ARGB);

	spr->Color().rmap = int2color(t.rmap, glue::PT_RGBA);
	spr->Color().gmap = int2color(t.gmap, glue::PT_RGBA);
	spr->Color().bmap = int2color(t.bmap, glue::PT_RGBA);
}


}