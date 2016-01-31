#ifndef _EASYEDITOR_SHAPE_MSG_H_
#define _EASYEDITOR_SHAPE_MSG_H_

#include "Subject.h"

namespace ee
{

class ClearShapeSJ : public Subject
{
public:
	void Clear(Observer* except = NULL);
	SUBJECT_DECLARATION(ClearShapeSJ)
}; // ClearShapeSJ

class Shape;
class InsertShapeSJ : public Subject
{
public:
	void Insert(Shape* shape, Observer* except = NULL);
	SUBJECT_DECLARATION(InsertShapeSJ)
}; // InsertShapeSJ

class RemoveShapeSJ : public Subject
{
public:
	void Remove(Shape* spr, Observer* except = NULL);
	SUBJECT_DECLARATION(RemoveShapeSJ)
}; // RemoveShapeSJ

class ShapeSelection;
class SelectShapeSetSJ : public Subject
{
public:
	void Selecte(const ShapeSelection* selection, Observer* except = NULL);
	SUBJECT_DECLARATION(SelectShapeSetSJ)
}; // SelectShapeSetSJ

class SelectShapeSJ : public Subject
{
public:
	void Select(Shape* shape, Observer* except = NULL);
	SUBJECT_DECLARATION(SelectShapeSJ)
}; // SelectShapeSJ

}

#endif // _EASYEDITOR_SHAPE_MSG_H_