#ifndef _DRAG2D_SELECT_SHAPE_SET_SJ_H_
#define _DRAG2D_SELECT_SHAPE_SET_SJ_H_

#include "Subject.h"

namespace d2d
{

class ShapeSelection;

class SelectShapeSetSJ : public Subject
{
public:
	void Selecte(const ShapeSelection* selection, Observer* except = NULL);

	static SelectShapeSetSJ* Instance();

private:
	SelectShapeSetSJ(int id);

private:
	static SelectShapeSetSJ* m_instance;

}; // SelectShapeSetSJ

}

#endif // _DRAG2D_SELECT_SHAPE_SET_SJ_H_