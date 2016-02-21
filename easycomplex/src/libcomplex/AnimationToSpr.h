#ifndef _EASYCOMPLEX_ANIMATION_TO_SPR_H_
#define _EASYCOMPLEX_ANIMATION_TO_SPR_H_


#include <easyrespacker.h>

namespace ecomplex
{

class AnimationToSpr
{
public:
	static ee::Sprite* Trans(const erespacker::PackAnimation* anim);

private:
	static ee::Sprite* TransComplex(const erespacker::PackAnimation* anim);
	static ee::Sprite* TransAnim(const erespacker::PackAnimation* anim);

	static void TransSprite(ee::Sprite* spr, const erespacker::PackAnimation::SpriteTrans& t);
	static void TransSpriteMat(ee::Sprite* spr, const erespacker::PackAnimation::SpriteTrans& t);
	static void TransSpriteCol(ee::Sprite* spr, const erespacker::PackAnimation::SpriteTrans& t);

}; // AnimationToSpr

}

#endif // _EASYCOMPLEX_ANIMATION_TO_SPR_H_