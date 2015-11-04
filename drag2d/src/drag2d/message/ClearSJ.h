#ifndef _DRAG2D_CLEAR_SJ_H_
#define _DRAG2D_CLEAR_SJ_H_

#include "Subject.h"

namespace d2d
{

class ISprite;

class ClearSJ : public Subject
{
public:
	void Clear(Observer* except = NULL);

	static ClearSJ* Instance();

private:
	ClearSJ(int id);

private:
	static ClearSJ* m_instance;

}; // ClearSJ

}

#endif // _DRAG2D_CLEAR_SJ_H_