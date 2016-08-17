#include "AnimSprite.h"

namespace s2
{

AnimSprite* AnimSprite::Clone() const
{
	return new AnimSprite(*this);
}

}