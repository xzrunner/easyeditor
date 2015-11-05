#ifndef _DRAG2D_INSERT_SHAPE_SJ_H_
#define _DRAG2D_INSERT_SHAPE_SJ_H_

#include "Subject.h"

namespace d2d
{

class IShape;

class InsertShapeSJ : public Subject
{
public:
	void Insert(d2d::IShape* shape, Observer* except = NULL);

	static InsertShapeSJ* Instance();

private:
	InsertShapeSJ(int id);

private:
	static InsertShapeSJ* m_instance;

}; // InsertShapeSJ

}

#endif // _DRAG2D_INSERT_SHAPE_SJ_H_