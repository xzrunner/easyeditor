#pragma once

#include <drag2d.h>

namespace eanim
{
	class Sprite
	{
	public:
		Sprite(Sprite* parent);
		Sprite(d2d::ISprite* sprite);
		~Sprite();

	public:
		d2d::ISprite* curr;
		Sprite* child;
	}; // Sprite
}