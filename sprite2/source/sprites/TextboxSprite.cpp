#include "TextboxSprite.h"

#include <stddef.h>

namespace s2
{

TextboxSprite* TextboxSprite::Clone() const
{
	return new TextboxSprite(*this);
}

}