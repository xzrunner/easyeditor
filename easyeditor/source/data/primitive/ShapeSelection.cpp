#include "ShapeSelection.h"
#include "panel_msg.h"

namespace ee
{

void ShapeSelection::Clear()
{
	SelectionSet<Shape>::Clear();
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void ShapeSelection::Add(const ShapePtr& item)
{
	SelectionSet<Shape>::Add(item);
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void ShapeSelection::Remove(const ShapePtr& item)
{
	SelectionSet<Shape>::Remove(item);
	SetCanvasDirtySJ::Instance()->SetDirty();
}

}