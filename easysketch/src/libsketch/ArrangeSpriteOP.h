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

	virtual bool onKeyDown(int keyCode);
	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseRightUp(int x, int y);
	virtual bool onMouseMove(int x, int y);
	virtual bool onMouseDrag(int x, int y);

private:
	IEditState* m_state;

	IEditState *m_translate_sprite, *m_rotate_sprite;
	IEditState *m_translate_camera, *m_rotate_camera;

}; // ArrangeSpriteOP

}

#endif // _LIBSKETCH_ARRANGE_SPRITE_OP_H_