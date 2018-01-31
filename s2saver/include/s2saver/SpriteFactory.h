#pragma once

#include <sns/typedef.h>

namespace s2 { class Sprite; }

namespace s2saver
{

class SpriteFactory
{
public:
	static sns::NodeSprPtr Create(const s2::Sprite& spr);

}; // SpriteFactory

}