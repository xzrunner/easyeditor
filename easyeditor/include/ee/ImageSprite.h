#ifndef _EASYEDITOR_IMAGE_SPRITE_H_
#define _EASYEDITOR_IMAGE_SPRITE_H_

#include "Sprite.h"

#include <sprite2/ImageSprite.h>
#include <sprite2/macro.h>

namespace ee
{

class ImageSymbol;

class ImageSprite : public s2::ImageSprite, public Sprite
{
public:
	ImageSprite(const ImageSprite& spr);
	ImageSprite& operator = (const ImageSprite& spr);
	ImageSprite(const s2::SymPtr& sym, uint32_t id = -1);

	// todo: should auto resize through its symbol and shapes
	void BuildBoundingFromTexCoords(float* texCoords);

	S2_SPR_CLONE_FUNC(ImageSprite)

}; // ImageSprite

}

#endif // _EASYEDITOR_IMAGE_SPRITE_H_