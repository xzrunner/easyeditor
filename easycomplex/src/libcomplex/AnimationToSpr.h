#ifndef _EASYCOMPLEX_ANIMATION_TO_SPR_H_
#define _EASYCOMPLEX_ANIMATION_TO_SPR_H_


#include <easyrespacker.h>

namespace ecomplex
{

class AnimationToSpr
{
public:
	static ee::SprPtr Trans(const erespacker::PackAnimation* anim);

private:
	static ee::SprPtr TransComplex(const erespacker::PackAnimation* anim);
	static ee::SprPtr TransAnim(const erespacker::PackAnimation* anim);

	static void TransSprite(const ee::SprPtr& spr, const erespacker::PackAnimation::SpriteTrans& t);
	static void TransSpriteMat(const ee::SprPtr& spr, const erespacker::PackAnimation::SpriteTrans& t);
	static void TransSpriteCol(const ee::SprPtr& spr, const erespacker::PackAnimation::SpriteTrans& t);

}; // AnimationToSpr

}

#endif // _EASYCOMPLEX_ANIMATION_TO_SPR_H_