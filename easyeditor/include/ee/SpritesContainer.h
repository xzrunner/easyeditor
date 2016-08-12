#ifndef _EASYEDITOR_SPRITES_CONTAINER_H_
#define _EASYEDITOR_SPRITES_CONTAINER_H_

#include "SprDataContainer.h"
#include "ObjectVector.h"

namespace ee
{

class Sprite;

class SpritesContainer : public SprDataContainer
{
public:
	virtual ~SpritesContainer();

	//
	// DataContainer interface
	//
	virtual void Traverse(Visitor<Sprite>& visitor, bool order = true) const;
	virtual void Traverse(Visitor<Sprite>& visitor, DataTraverseType type = DT_ALL, bool order = true) const;
	virtual bool Remove(Sprite* spr);
	virtual bool Insert(Sprite* spr);
	virtual bool Insert(Sprite* spr, int idx);
	virtual bool Clear();
	virtual bool ResetOrder(const Sprite* spr, bool up);
	virtual bool ResetOrderMost(const Sprite* spr, bool up);
	//
	// SprDataContainer interface
	//
	virtual bool Sort(std::vector<Sprite*>& sprites);

	int Size() const;

private:
	ObjectVector<Sprite> m_sprites;

}; // SpritesContainer

}

#endif // _EASYEDITOR_SPRITES_CONTAINER_H_