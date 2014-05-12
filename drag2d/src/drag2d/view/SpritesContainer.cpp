#include "SpritesContainer.h"

#include "dataset/ISprite.h"

#include "VectorUtils.h"

namespace d2d
{
	SpritesContainer::~SpritesContainer()
	{
		clear();
	}

	void SpritesContainer::traverse(IVisitor& visitor, bool order/* = true*/) const
	{
		VectorUtils::traverse(m_sprites, visitor, order);
	}

	void SpritesContainer::remove(Object* obj)
	{
		VectorUtils::remove(m_sprites, static_cast<ISprite*>(obj));
	}

	void SpritesContainer::insert(Object* obj)
	{
		VectorUtils::insert(m_sprites, static_cast<ISprite*>(obj));
	}

	void SpritesContainer::clear()
	{
		VectorUtils::clear(m_sprites);
	}

	void SpritesContainer::resetOrder(const Object* obj, bool up)
	{
		VectorUtils::resetOrder(m_sprites, static_cast<const ISprite*>(obj), up);
	}
}