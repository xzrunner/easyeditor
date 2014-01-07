#include "SpriteObserver.h"

namespace eanim
{

void SpriteObserver::translate(d2d::ISprite* sprite, const d2d::Vector& offset)
{
	std::map<const d2d::ISprite*, Sprite*>::iterator itr = m_mapSprite.find(sprite);
	if (itr != m_mapSprite.end() && itr->second->child) {
		itr->second->child->curr->translate(offset);
	}
}

void SpriteObserver::rotate(d2d::ISprite* sprite, float delta)
{
	std::map<const d2d::ISprite*, Sprite*>::iterator itr = m_mapSprite.find(sprite);
	if (itr != m_mapSprite.end() && itr->second->child) {
		itr->second->child->curr->rotate(delta);
	}
}

void SpriteObserver::insert(Sprite* sprite)
{
	m_mapSprite.insert(std::make_pair(sprite->curr, sprite));
}

void SpriteObserver::remove(const d2d::ISprite* sprite)
{
	std::map<const d2d::ISprite*, Sprite*>::iterator itr 
		= m_mapSprite.find(sprite);
	if (itr != m_mapSprite.end())
		m_mapSprite.erase(itr);
}

}