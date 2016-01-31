#include "shape_msg.h"
#include "subject_id.h"

namespace ee
{

SUBJECT_DEFINITION(ClearShapeSJ, MSG_CLEAR_SHAPE)
void ClearShapeSJ::Clear(Observer* except)
{
	Notify(NULL, except);
}

SUBJECT_DEFINITION(InsertShapeSJ, MSG_INSERT_SHAPE)
void InsertShapeSJ::Insert(Shape* shape, Observer* except)
{
	Notify((void*)shape, except);
}

SUBJECT_DEFINITION(RemoveShapeSJ, MSG_REMOVE_SHAPE)
void RemoveShapeSJ::Remove(Shape* spr, Observer* except)
{
	Notify((void*)spr, except);
}

SUBJECT_DEFINITION(SelectShapeSetSJ, MSG_SELECT_SHAPE_SET)
void SelectShapeSetSJ::Selecte(const ShapeSelection* selection, Observer* except)
{
	Notify((void*)selection, except);
}

SUBJECT_DEFINITION(SelectShapeSJ, MSG_SELECT_SHAPE)
void SelectShapeSJ::Select(Shape* shape, Observer* except)
{
	Notify((void*)shape, except);
}

}