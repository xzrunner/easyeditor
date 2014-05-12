#pragma once

#include "IDataContainer.h"
#include "VectorUtils.h"

namespace d2d
{
	class ISprite;

	class SpritesContainer : public IDataContainer
	{
	public:
		virtual ~SpritesContainer();

		//
		// IDataContainer interface
		//
		virtual void traverse(IVisitor& visitor, bool order = true) const;
		virtual void remove(Object* obj);
		virtual void insert(Object* obj);
		virtual void clear();
		virtual void resetOrder(const Object* obj, bool up);

	private:
		std::vector<ISprite*> m_sprites;

	}; // SpritesContainer
}