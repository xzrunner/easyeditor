#pragma once

#include <s2s/typedef.h>

namespace s2 { class Sprite; }

namespace s2storer
{

class SpriteFactory
{
public:
	static s2s::NodeSprPtr Create(const s2::Sprite& spr);

}; // SpriteFactory

}