#ifndef _DRAG2D_SPRITES_CONTAINER_H_
#define _DRAG2D_SPRITES_CONTAINER_H_

#include "IDataContainer.h"
#include "ObjectVector.h"

namespace d2d
{

class Sprite;

class SpritesContainer : public IDataContainer
{
public:
	virtual ~SpritesContainer();

	//
	// IDataContainer interface
	//
	virtual void Traverse(Visitor& visitor, bool order = true) const;
	virtual void Traverse(Visitor& visitor, DataTraverseType type = DT_ALL, bool order = true) const;
	virtual bool Remove(Object* obj);
	virtual bool Insert(Object* obj);
	virtual bool Insert(Object* obj, int idx);
	virtual bool Clear();
	virtual bool ResetOrder(const Object* obj, bool up);
	virtual bool ResetOrderMost(const Object* obj, bool up);

	int Size() const;

private:
	ObjectVector<Sprite> m_sprites;

}; // SpritesContainer

}

#endif // _DRAG2D_SPRITES_CONTAINER_H_