#ifndef _DRAG2D_REMOVE_SHAPE_SJ_H_
#define _DRAG2D_REMOVE_SHAPE_SJ_H_

#include "Subject.h"

namespace d2d
{

class IShape;

class RemoveShapeSJ : public Subject
{
public:
	void Remove(IShape* spr, Observer* except = NULL);

	static RemoveShapeSJ* Instance();

private:
	RemoveShapeSJ(int id);

private:
	static RemoveShapeSJ* m_instance;

}; // RemoveShapeSJ

}

#endif // _DRAG2D_REMOVE_SHAPE_SJ_H_