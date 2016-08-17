#include "ImageSprite.h"

#include <stddef.h>

namespace s2
{

ImageSprite* ImageSprite::Clone() const
{
	return new ImageSprite(*this);
}

}