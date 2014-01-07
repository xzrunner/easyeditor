#pragma once

#include <drag2d.h>

#include "Sprite.h"

namespace eanim
{
	class SpriteObserver : public d2d::ISpriteObserver
	{
	public:

		virtual void translate(d2d::ISprite* sprite, const d2d::Vector& offset);
		virtual void rotate(d2d::ISprite* sprite, float delta);

		void insert(Sprite* sprite);
		void remove(const d2d::ISprite* sprite);

	private:
		std::map<const d2d::ISprite*, Sprite*> m_mapSprite;

	}; // SpriteObserver
}