#include "Scale9Sprite.h"
#include "Scale9Symbol.h"

#include <stddef.h>

namespace s2
{

Scale9Sprite::Scale9Sprite() 
{

}

Scale9Sprite::Scale9Sprite(Symbol* sym) 
	: Sprite(sym)
	, m_s9(VI_DOWNCASTING<Scale9Symbol*>(sym)->GetScale9())
{
}

Scale9Sprite* Scale9Sprite::Clone() const
{
	return new Scale9Sprite(*this);
}

}