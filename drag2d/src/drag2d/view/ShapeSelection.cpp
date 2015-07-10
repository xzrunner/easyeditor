#include "ShapeSelection.h"
#include "IStageCanvas.h"

#include "view/EditPanel.h"

namespace d2d
{

ShapeSelection::ShapeSelection(EditPanel* stage)
	: m_stage(stage)
{
}

void ShapeSelection::Clear()
{
	ObjSelectionSet<IShape>::Clear();
	m_stage->SetCanvasDirty();
}

void ShapeSelection::Add(IShape* item)
{
	ObjSelectionSet<IShape>::Add(item);
	m_stage->SetCanvasDirty();
}

void ShapeSelection::Remove(IShape* item)
{
	ObjSelectionSet<IShape>::Remove(item);
	m_stage->SetCanvasDirty();
}

}