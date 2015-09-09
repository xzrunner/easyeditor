#ifndef _LIBCOMPLEX_ANIMATION_TO_SPR_H_
#define _LIBCOMPLEX_ANIMATION_TO_SPR_H_

#include <drag2d.h>
#include <easycoco.h>

namespace ecomplex
{

class AnimationToSpr
{
public:
	static d2d::ISprite* Trans(const libcoco::PackAnimation* anim);

private:
	static d2d::ISprite* TransComplex(const libcoco::PackAnimation* anim);
	static d2d::ISprite* TransAnim(const libcoco::PackAnimation* anim);

	static void TransSprite(d2d::ISprite* spr, const libcoco::PackAnimation::SpriteTrans& t);
	static void TransSpriteMat(d2d::ISprite* spr, const libcoco::PackAnimation::SpriteTrans& t);
	static void TransSpriteCol(d2d::ISprite* spr, const libcoco::PackAnimation::SpriteTrans& t);

}; // AnimationToSpr

}

#endif // _LIBCOMPLEX_ANIMATION_TO_SPR_H_