#include "IconSprite.h"

#include <stddef.h>

namespace s2
{

IconSprite* IconSprite::Clone() const
{
	return new IconSprite(*this);
}

}