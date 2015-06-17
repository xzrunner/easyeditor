#ifndef _LIBSKETCH_ARRANGE_SPRITE_OP_H_
#define _LIBSKETCH_ARRANGE_SPRITE_OP_H_

#include "SelectSpriteOP.h"

namespace libsketch
{

class IEditState;

class ArrangeSpriteOP : public SelectSpriteOP
{
public:
	ArrangeSpriteOP(StagePanel* stage);
	virtual ~ArrangeSpriteOP();

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

private:
	IEditState* m_state;

	IEditState *m_translate_sprite, *m_rotate_sprite;
	IEditState *m_translate_camera, *m_rotate_camera;

}; // ArrangeSpriteOP

}

#endif // _LIBSKETCH_ARRANGE_SPRITE_OP_H_