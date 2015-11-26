#ifndef _DRAG2D_SET_CANVAS_DIRTY_SJ_H_
#define _DRAG2D_SET_CANVAS_DIRTY_SJ_H_

#include "Subject.h"

namespace d2d
{

class SetCanvasDirtySJ : public Subject
{
public:
	void SetDirty();

	static SetCanvasDirtySJ* Instance();

private:
	SetCanvasDirtySJ(int id);

private:
	static SetCanvasDirtySJ* m_instance;

}; // SetCanvasDirtySJ

}

#endif // _DRAG2D_SET_CANVAS_DIRTY_SJ_H_