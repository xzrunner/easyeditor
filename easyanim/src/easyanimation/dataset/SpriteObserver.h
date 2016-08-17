#ifndef _EASYANIM_SPRITE_OBSERVER_H_
#define _EASYANIM_SPRITE_OBSERVER_H_

#include <ee/SpriteObserver.h>

#include <map>

namespace eanim
{

class Layer;

class SpriteObserver : public ee::SpriteObserver
{
public:
	SpriteObserver(const Layer& layer);

	virtual void Translate(ee::Sprite* spr, const sm::vec2& offset);
	virtual void Rotate(ee::Sprite* spr, float delta);

	void Insert(const ee::Sprite* spr, int frame);
	void Remove(const ee::Sprite* spr);

	void Enable(bool enable) { m_enable = enable; }

private:
	const Layer& m_layer;

	std::map<ee::Sprite*, int> m_map2frame;

	bool m_enable;

}; // SpriteObserver

}

#endif // _EASYANIM_SPRITE_OBSERVER_H_