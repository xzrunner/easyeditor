#ifndef _EASYANIM_SPRITE_OBSERVER_H_
#define _EASYANIM_SPRITE_OBSERVER_H_

#include <drag2d.h>

namespace eanim
{

class Layer;

class SpriteObserver : public d2d::ISpriteObserver
{
public:
	SpriteObserver(const Layer& layer);

	virtual void Translate(d2d::ISprite* sprite, const d2d::Vector& offset);
	virtual void Rotate(d2d::ISprite* sprite, float delta);

	void insert(const d2d::ISprite* sprite, int frame);
	void remove(const d2d::ISprite* sprite);

private:
	const Layer& m_layer;

	std::map<d2d::ISprite*, int> m_map2frame;

	bool m_enable;

}; // SpriteObserver

}

#endif // _EASYANIM_SPRITE_OBSERVER_H_