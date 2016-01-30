#ifndef _DRAG2D_CLEAR_SPRITE_SJ_H_
#define _DRAG2D_CLEAR_SPRITE_SJ_H_

#include "Subject.h"

namespace d2d
{

class Sprite;

class ClearSpriteSJ : public Subject
{
public:
	void Clear(Observer* except = NULL);

	static ClearSpriteSJ* Instance();

private:
	ClearSpriteSJ(int id);

private:
	static ClearSpriteSJ* m_instance;

}; // ClearSpriteSJ

}

#endif // _DRAG2D_CLEAR_SPRITE_SJ_H_