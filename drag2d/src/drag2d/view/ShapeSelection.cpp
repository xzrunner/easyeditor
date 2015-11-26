#include "ShapeSelection.h"
#include "IStageCanvas.h"
#include "message/SetCanvasDirtySJ.h"

namespace d2d
{

void ShapeSelection::Clear()
{
	ObjSelectionSet<IShape>::Clear();
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void ShapeSelection::Add(IShape* item)
{
	ObjSelectionSet<IShape>::Add(item);
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void ShapeSelection::Remove(IShape* item)
{
	ObjSelectionSet<IShape>::Remove(item);
	SetCanvasDirtySJ::Instance()->SetDirty();
}

}