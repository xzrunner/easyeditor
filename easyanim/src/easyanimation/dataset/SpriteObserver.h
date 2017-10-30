#ifndef _EASYANIM_SPRITE_OBSERVER_H_
#define _EASYANIM_SPRITE_OBSERVER_H_

#include <ee/SpriteObserver.h>
#include <ee/Sprite.h>

#include <map>

namespace eanim
{

class Layer;

class SpriteObserver : public ee::SpriteObserver
{
public:
	SpriteObserver(const Layer& layer);

	virtual void OnSetPosition(const ee::Sprite& spr, const sm::vec2& pos);
	virtual void OnSetAngle(const ee::Sprite& spr, float angle);
	virtual void OnSetScale(const ee::Sprite& spr, const sm::vec2& scale);
	virtual void OnSetShear(const ee::Sprite& spr, const sm::vec2& shear);
	virtual void OnSetOffset(const ee::Sprite& spr, const sm::vec2& offset);

	void Insert(const ee::SprPtr& spr, int frame);
	void Remove(const ee::SprPtr& spr);

	void Enable(bool enable) { m_enable = enable; }

private:
	const Layer& m_layer;

	// spr id -> frame
	std::map<int, int> m_map2frame;

	bool m_enable;

}; // SpriteObserver

}

#endif // _EASYANIM_SPRITE_OBSERVER_H_