#ifndef _LIBCOMPLEX_NODE_TO_SPRITE_H_
#define _LIBCOMPLEX_NODE_TO_SPRITE_H_

#include <drag2d.h>
#include <easycoco.h>

namespace ecomplex
{

class NodeToSprite
{
public:
	static d2d::ISprite* ToSprite(const libcoco::IPackNode* node);

private:
	static d2d::ISprite* Quad2Sprite(const libcoco::PackPicture::Quad* quad);
	static d2d::ISprite* Pic2Sprite(const libcoco::PackPicture* pic);
	static d2d::ISprite* Label2Sprite(const libcoco::PackLabel* label);
	static d2d::ISprite* Anim2Sprite(const libcoco::PackAnimation* anim);
	static d2d::ISprite* Anim2ComplexSprite(const libcoco::PackAnimation* anim);
	static d2d::ISprite* Anim2AnimSprite(const libcoco::PackAnimation* anim);

	static void TransSprite(d2d::ISprite* spr, const libcoco::PackAnimation::SpriteTrans& t);
	static void TransSpriteMat(d2d::ISprite* spr, const libcoco::PackAnimation::SpriteTrans& t);
	static void TransSpriteCol(d2d::ISprite* spr, const libcoco::PackAnimation::SpriteTrans& t);

};	// NodeToSprite

}

#endif // _LIBCOMPLEX_NODE_TO_SPRITE_H_