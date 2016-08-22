#ifndef _EASYEDITOR_IMAGE_SPRITE_H_
#define _EASYEDITOR_IMAGE_SPRITE_H_

#include "Sprite.h"

#include <sprite2/ImageSprite.h>

namespace ee
{

class ImageSymbol;

class ImageSprite : public s2::ImageSprite, public Sprite
{
public:
	ImageSprite(const ImageSprite& spr);
	ImageSprite(ImageSymbol* sym);

	/**
	 *  @interface
	 *    s2::Sprite
	 */
	virtual ImageSprite* Clone() const { return new ImageSprite(*this); }
	virtual bool Update(const s2::RenderParams& params, float dt);	

	// todo: should auto resize through its symbol and shapes
	void BuildBoundingFromTexCoords(float* texCoords);

}; // ImageSprite

}

#endif // _EASYEDITOR_IMAGE_SPRITE_H_