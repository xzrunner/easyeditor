#ifndef _DRAG2D_SPRITES_CONTAINER_H_
#define _DRAG2D_SPRITES_CONTAINER_H_

#include "IDataContainer.h"

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
	virtual void traverse(IVisitor& visitor, TraverseType type = e_allExisting, bool order = true) const;
	virtual void remove(Object* obj);
	virtual void insert(Object* obj);
	virtual void clear();
	virtual void resetOrder(const Object* obj, bool up);

private:
	std::vector<ISprite*> m_sprites;

}; // SpritesContainer

}

#endif // _DRAG2D_SPRITES_CONTAINER_H_