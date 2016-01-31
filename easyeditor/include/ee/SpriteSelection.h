#ifndef _EASYEDITOR_SPRITE_SELECTION_H_
#define _EASYEDITOR_SPRITE_SELECTION_H_

#include "Sprite.h"
#include "ObjSelectionSet.h"

namespace ee
{

class SpriteSelection : public ObjSelectionSet<Sprite>
{
public:
	virtual void Clear();

	virtual void Add(Sprite* item);
	virtual void Remove(Sprite* item);

}; // SpriteSelection

}

#endif // _EASYEDITOR_SPRITE_SELECTION_H_