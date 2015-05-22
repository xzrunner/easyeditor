#include "SpriteObserver.h"
#include "KeyFrame.h"
#include "Layer.h"

namespace eanim
{

SpriteObserver::SpriteObserver(const Layer& layer)
	: m_layer(layer)
	, m_enable(true)
{
}

void SpriteObserver::translate(d2d::ISprite* sprite, const d2d::Vector& offset)
{
	if (!m_enable) {
		return;
	}

	std::map<d2d::ISprite*, int>::iterator itr_sprite = m_map2Frame.find(sprite);
	if (itr_sprite == m_map2Frame.end())
		return;

	m_enable = false;

	const std::map<int, KeyFrame*>& frames = m_layer.getAllFrames();
	std::map<int, KeyFrame*>::const_iterator itr_frame = frames.upper_bound(itr_sprite->second);
	for ( ; itr_frame != frames.end(); ++itr_frame)
	{
		KeyFrame* frame = itr_frame->second;
		const std::vector<d2d::ISprite*>& sprites = frame->GetAllSprites();
		for (int i = 0, n = sprites.size(); i < n; ++i)
		{
			if (sprites[i]->name == sprite->name)
				sprites[i]->translate(offset);
		}
	}

	m_enable = true;
}

void SpriteObserver::rotate(d2d::ISprite* sprite, float delta)
{
	if (!m_enable) {
		return;
	}

	std::map<d2d::ISprite*, int>::iterator itr_sprite = m_map2Frame.find(sprite);
	if (itr_sprite == m_map2Frame.end())
		return;

	m_enable = false;

	const std::map<int, KeyFrame*>& frames = m_layer.getAllFrames();
	std::map<int, KeyFrame*>::const_iterator itr_frame = frames.upper_bound(itr_sprite->second);
	for ( ; itr_frame != frames.end(); ++itr_frame)
	{
		KeyFrame* frame = itr_frame->second;
		const std::vector<d2d::ISprite*>& sprites = frame->GetAllSprites();
		for (int i = 0, n = sprites.size(); i < n; ++i)
		{
			if (sprites[i]->name == sprite->name)
				sprites[i]->rotate(delta);
		}
	}

	m_enable = true;
}

void SpriteObserver::insert(const d2d::ISprite* sprite, int frame)
{
	m_map2Frame.insert(std::make_pair(const_cast<d2d::ISprite*>(sprite), frame));
}

void SpriteObserver::remove(const d2d::ISprite* sprite)
{
	std::map<d2d::ISprite*, int>::iterator itr 
		= m_map2Frame.find(const_cast<d2d::ISprite*>(sprite));
	if (itr != m_map2Frame.end())
		m_map2Frame.erase(itr);
}

}