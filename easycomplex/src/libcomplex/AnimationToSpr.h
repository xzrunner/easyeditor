#ifndef _LIBCOMPLEX_ANIMATION_TO_SPR_H_
#define _LIBCOMPLEX_ANIMATION_TO_SPR_H_

#include <drag2d.h>
#include <easyrespacker.h>

namespace ecomplex
{

class AnimationToSpr
{
public:
	static d2d::Sprite* Trans(const librespacker::PackAnimation* anim);

private:
	static d2d::Sprite* TransComplex(const librespacker::PackAnimation* anim);
	static d2d::Sprite* TransAnim(const librespacker::PackAnimation* anim);

	static void TransSprite(d2d::Sprite* spr, const librespacker::PackAnimation::SpriteTrans& t);
	static void TransSpriteMat(d2d::Sprite* spr, const librespacker::PackAnimation::SpriteTrans& t);
	static void TransSpriteCol(d2d::Sprite* spr, const librespacker::PackAnimation::SpriteTrans& t);

}; // AnimationToSpr

}

#endif // _LIBCOMPLEX_ANIMATION_TO_SPR_H_