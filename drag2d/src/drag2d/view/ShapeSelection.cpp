#include "ShapeSelection.h"
#include "IStageCanvas.h"

#include "view/EditPanelImpl.h"

namespace d2d
{

ShapeSelection::ShapeSelection(EditPanelImpl* stage)
{
	if (stage) {
		stage->Retain();
	}	
	m_stage = stage;
}

ShapeSelection::~ShapeSelection()
{
	if (m_stage) {
		m_stage->Release();
	}
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