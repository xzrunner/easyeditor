#include "SpriteObserver.h"
#include "KeyFrame.h"
#include "Layer.h"

#include <ee/Sprite.h>

namespace eanim
{

SpriteObserver::SpriteObserver(const Layer& layer)
	: m_layer(layer)
	, m_enable(false)
{
}

void SpriteObserver::Translate(ee::Sprite* spr, const sm::vec2& offset)
{
	if (!m_enable) {
		return;
	}

	std::map<ee::Sprite*, int>::iterator itr_sprite = m_map2frame.find(spr);
	if (itr_sprite == m_map2frame.end())
		return;

	m_enable = false;

	const std::map<int, KeyFrame*>& frames = m_layer.GetAllFrames();
	std::map<int, KeyFrame*>::const_iterator itr_frame = frames.upper_bound(itr_sprite->second);
	for ( ; itr_frame != frames.end(); ++itr_frame)
	{
		KeyFrame* frame = itr_frame->second;
		const std::vector<ee::Sprite*>& sprs = frame->GetAllSprites();
		for (int i = 0, n = sprs.size(); i < n; ++i)
		{
			if (sprs[i]->GetName() == spr->GetName())
				sprs[i]->Translate(offset);
		}
	}

	m_enable = true;
}

void SpriteObserver::Rotate(ee::Sprite* spr, float delta)
{
	if (!m_enable) {
		return;
	}

	std::map<ee::Sprite*, int>::iterator itr_sprite = m_map2frame.find(spr);
	if (itr_sprite == m_map2frame.end())
		return;

	m_enable = false;

	const std::map<int, KeyFrame*>& frames = m_layer.GetAllFrames();
	std::map<int, KeyFrame*>::const_iterator itr_frame = frames.upper_bound(itr_sprite->second);
	for ( ; itr_frame != frames.end(); ++itr_frame)
	{
		KeyFrame* frame = itr_frame->second;
		const std::vector<ee::Sprite*>& sprs = frame->GetAllSprites();
		for (int i = 0, n = sprs.size(); i < n; ++i)
		{
			if (sprs[i]->GetName() == spr->GetName())
				sprs[i]->Rotate(delta);
		}
	}

	m_enable = true;
}

void SpriteObserver::Insert(const ee::Sprite* spr, int frame)
{
	m_map2frame.insert(std::make_pair(const_cast<ee::Sprite*>(spr), frame));
}

void SpriteObserver::Remove(const ee::Sprite* spr)
{
	std::map<ee::Sprite*, int>::iterator itr 
		= m_map2frame.find(const_cast<ee::Sprite*>(spr));
	if (itr != m_map2frame.end())
		m_map2frame.erase(itr);
}

}