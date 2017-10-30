#ifndef _EASYEDITOR_SPRITE_SELECTION_H_
#define _EASYEDITOR_SPRITE_SELECTION_H_

#include "Sprite.h"
#include "SelectionSet.h"

namespace ee
{

class SpriteSelection : public SelectionSet<Sprite>
{
public:
	virtual void Clear();

	virtual void Add(const SprPtr& item);
	virtual void Remove(const SprPtr& item);

}; // SpriteSelection

}

#endif // _EASYEDITOR_SPRITE_SELECTION_H_