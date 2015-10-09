#ifndef _LIBCOMPLEX_ANIMATION_TO_SPR_H_
#define _LIBCOMPLEX_ANIMATION_TO_SPR_H_

#include <drag2d.h>
#include <easyrespacker.h>

namespace ecomplex
{

class AnimationToSpr
{
public:
	static d2d::ISprite* Trans(const librespacker::PackAnimation* anim);

private:
	static d2d::ISprite* TransComplex(const librespacker::PackAnimation* anim);
	static d2d::ISprite* TransAnim(const librespacker::PackAnimation* anim);

	static void TransSprite(d2d::ISprite* spr, const librespacker::PackAnimation::SpriteTrans& t);
	static void TransSpriteMat(d2d::ISprite* spr, const librespacker::PackAnimation::SpriteTrans& t);
	static void TransSpriteCol(d2d::ISprite* spr, const librespacker::PackAnimation::SpriteTrans& t);

}; // AnimationToSpr

}

#endif // _LIBCOMPLEX_ANIMATION_TO_SPR_H_