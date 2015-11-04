#ifndef _DRAG2D_SELECTE_SHAPE_SJ_H_
#define _DRAG2D_SELECTE_SHAPE_SJ_H_

#include "Subject.h"

namespace d2d
{

class IShape;

class SelectShapeSJ : public Subject
{
public:
	void Select(IShape* shape, Observer* except = NULL);

	static SelectShapeSJ* Instance();

private:
	SelectShapeSJ(int id);

private:
	static SelectShapeSJ* m_instance;

}; // SelectShapeSJ

}

#endif // _DRAG2D_SELECTE_SHAPE_SJ_H_