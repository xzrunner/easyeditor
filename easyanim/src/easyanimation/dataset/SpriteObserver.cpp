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

void SpriteObserver::Translate(d2d::Sprite* sprite, const d2d::Vector& offset)
{
	if (!m_enable) {
		return;
	}

	std::map<d2d::Sprite*, int>::iterator itr_sprite = m_map2frame.find(sprite);
	if (itr_sprite == m_map2frame.end())
		return;

	m_enable = false;

	const std::map<int, KeyFrame*>& frames = m_layer.GetAllFrames();
	std::map<int, KeyFrame*>::const_iterator itr_frame = frames.upper_bound(itr_sprite->second);
	for ( ; itr_frame != frames.end(); ++itr_frame)
	{
		KeyFrame* frame = itr_frame->second;
		const std::vector<d2d::Sprite*>& sprites = frame->GetAllSprites();
		for (int i = 0, n = sprites.size(); i < n; ++i)
		{
			if (sprites[i]->name == sprite->name)
				sprites[i]->Translate(offset);
		}
	}

	m_enable = true;
}

void SpriteObserver::Rotate(d2d::Sprite* sprite, float delta)
{
	if (!m_enable) {
		return;
	}

	std::map<d2d::Sprite*, int>::iterator itr_sprite = m_map2frame.find(sprite);
	if (itr_sprite == m_map2frame.end())
		return;

	m_enable = false;

	const std::map<int, KeyFrame*>& frames = m_layer.GetAllFrames();
	std::map<int, KeyFrame*>::const_iterator itr_frame = frames.upper_bound(itr_sprite->second);
	for ( ; itr_frame != frames.end(); ++itr_frame)
	{
		KeyFrame* frame = itr_frame->second;
		const std::vector<d2d::Sprite*>& sprites = frame->GetAllSprites();
		for (int i = 0, n = sprites.size(); i < n; ++i)
		{
			if (sprites[i]->name == sprite->name)
				sprites[i]->Rotate(delta);
		}
	}

	m_enable = true;
}

void SpriteObserver::insert(const d2d::Sprite* sprite, int frame)
{
	m_map2frame.insert(std::make_pair(const_cast<d2d::Sprite*>(sprite), frame));
}

void SpriteObserver::remove(const d2d::Sprite* sprite)
{
	std::map<d2d::Sprite*, int>::iterator itr 
		= m_map2frame.find(const_cast<d2d::Sprite*>(sprite));
	if (itr != m_map2frame.end())
		m_map2frame.erase(itr);
}

}