#include "Scale9Data.h"

namespace escale9
{

void Scale9Data::Resize()
{
	
}

void Scale9Data::ResizeSprite(d2d::ISprite* sprite, const d2d::Vector& center, 
							  float width, float height)
{
	const d2d::ImageSymbol& symbol = dynamic_cast<const d2d::ImageSymbol&>(sprite->getSymbol());
	int w = symbol.getImage()->originWidth(),
		h = symbol.getImage()->originHeight();
	assert(w != 0 && h != 0);

	sprite->setTransform(center, sprite->getAngle());
	const float times = sprite->getAngle() / d2d::PI;
	if (times - (int)(times + 0.01f) < 0.3f)
		sprite->setScale(width / w, height / h);
	else
		sprite->setScale(height / w, width / h);

	sprite->translate(d2d::Math::rotateVector(sprite->getOffset(), sprite->getAngle()) - sprite->getOffset());
}

}