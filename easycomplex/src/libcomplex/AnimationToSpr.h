#ifndef _LIBCOMPLEX_ANIMATION_TO_SPR_H_
#define _LIBCOMPLEX_ANIMATION_TO_SPR_H_


#include <easyrespacker.h>

namespace ecomplex
{

class AnimationToSpr
{
public:
	static ee::Sprite* Trans(const librespacker::PackAnimation* anim);

private:
	static ee::Sprite* TransComplex(const librespacker::PackAnimation* anim);
	static ee::Sprite* TransAnim(const librespacker::PackAnimation* anim);

	static void TransSprite(ee::Sprite* spr, const librespacker::PackAnimation::SpriteTrans& t);
	static void TransSpriteMat(ee::Sprite* spr, const librespacker::PackAnimation::SpriteTrans& t);
	static void TransSpriteCol(ee::Sprite* spr, const librespacker::PackAnimation::SpriteTrans& t);

}; // AnimationToSpr

}

#endif // _LIBCOMPLEX_ANIMATION_TO_SPR_H_