#include "Sprite.h"

namespace eanim
{

Sprite::Sprite(Sprite* parent) 
	: curr(parent->curr->clone()), child(NULL)
{
	parent->child = this;
}

Sprite::Sprite(d2d::ISprite* sprite)
	: curr(sprite)
	, child(NULL)
{
}

Sprite::~Sprite()
{
	curr->release();
}

}