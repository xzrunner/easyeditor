#ifndef _EASYEDITOR_SPRITE_SELECTION_H_
#define _EASYEDITOR_SPRITE_SELECTION_H_

#include "Sprite.h"
#include "SelectionSet.h"

namespace ee
{

class SpriteSelection : public SelectionSet<Sprite>
{
public:
	virtual void Clear() override;

	virtual void Add(const SprPtr& item) override;
	virtual void Remove(const SprPtr& item) override;

}; // SpriteSelection

}

#endif // _EASYEDITOR_SPRITE_SELECTION_H_