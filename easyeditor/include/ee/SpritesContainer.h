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
	virtual void Traverse(RefVisitor<Sprite>& visitor, bool order = true) const override;
	virtual void Traverse(RefVisitor<Sprite>& visitor, DataTraverseType type = DT_ALL, bool order = true) const override;
	virtual bool Remove(const SprPtr& spr) override;
	virtual bool Insert(const SprPtr& spr) override;
	virtual bool Insert(const SprPtr& spr, int idx) override;
	virtual bool Clear() override;
	virtual bool ResetOrder(const SprConstPtr& spr, bool up) override;
	virtual bool ResetOrderMost(const SprConstPtr& spr, bool up) override;
	//
	// SprDataContainer interface
	//
	virtual bool Sort(std::vector<SprPtr>& sprs) override;

	int Size() const;

private:
	ObjectVector<Sprite> m_sprs;

}; // SpritesContainer

}

#endif // _EASYEDITOR_SPRITES_CONTAINER_H_