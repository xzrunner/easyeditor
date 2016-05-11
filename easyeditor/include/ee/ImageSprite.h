#ifndef _EASYEDITOR_IMAGE_SPRITE_H_
#define _EASYEDITOR_IMAGE_SPRITE_H_

#include "Sprite.h"
#include "ImageSymbol.h"

namespace ee
{

class ImageSymbol;

class ImageSprite : public Sprite
{
public:
	ImageSprite();
	ImageSprite(const ImageSprite& sprite);
	ImageSprite(ImageSymbol* symbol);
	virtual ~ImageSprite();

	//
	// Cloneable interface
	//
	virtual ImageSprite* Clone() const;

	//
	// Sprite interface
	//
	virtual bool Update(float dt);
	virtual const ImageSymbol& GetSymbol() const;
	virtual void SetSymbol(Symbol* symbol);

	// todo: should auto resize through its symbol and shapes
	void BuildBoundingFromTexCoords(float* texCoords);

protected:
	ImageSymbol* m_symbol;

}; // ImageSprite

}

#endif // _EASYEDITOR_IMAGE_SPRITE_H_