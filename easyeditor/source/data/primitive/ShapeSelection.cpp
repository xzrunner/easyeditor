#include "ShapeSelection.h"
#include "panel_msg.h"

namespace ee
{

void ShapeSelection::Clear()
{
	ObjSelectionSet<Shape>::Clear();
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void ShapeSelection::Add(Shape* item)
{
	ObjSelectionSet<Shape>::Add(item);
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void ShapeSelection::Remove(Shape* item)
{
	ObjSelectionSet<Shape>::Remove(item);
	SetCanvasDirtySJ::Instance()->SetDirty();
}

}