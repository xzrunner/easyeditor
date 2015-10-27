#ifndef _DRAG2D_SPRITES_CONTAINER_H_
#define _DRAG2D_SPRITES_CONTAINER_H_

#include "IDataContainer.h"
#include "ObjectVector.h"

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
	virtual void Traverse(IVisitor& visitor, bool order = true) const;
	virtual void Traverse(IVisitor& visitor, DataTraverseType type = DT_ALL, bool order = true) const;
	virtual bool Remove(Object* obj);
	virtual bool Insert(Object* obj);
	virtual bool Insert(Object* obj, int idx);
	virtual bool Clear();
	virtual bool ResetOrder(const Object* obj, bool up);

	int Size() const;

private:
	ObjectVector<ISprite> m_sprites;

}; // SpritesContainer

}

#endif // _DRAG2D_SPRITES_CONTAINER_H_