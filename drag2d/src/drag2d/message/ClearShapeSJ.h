#ifndef _DRAG2D_CLEAR_SHAPE_SJ_H_
#define _DRAG2D_CLEAR_SHAPE_SJ_H_

#include "Subject.h"

namespace d2d
{

class IShape;

class ClearShapeSJ : public Subject
{
public:
	void Clear(Observer* except = NULL);

	static ClearShapeSJ* Instance();

private:
	ClearShapeSJ(int id);

private:
	static ClearShapeSJ* m_instance;

}; // ClearShapeSJ

}

#endif // _DRAG2D_CLEAR_SHAPE_SJ_H_