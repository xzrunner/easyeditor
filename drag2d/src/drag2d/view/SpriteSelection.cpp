#include "SpriteSelection.h"
#include "IStageCanvas.h"

#include "view/EditPanelImpl.h"

namespace d2d
{

SpriteSelection::SpriteSelection(EditPanelImpl* stage)
{
	if (stage) {
		stage->Retain();
	}
	m_stage = stage;
}

SpriteSelection::~SpriteSelection()
{
	if (m_stage) {
		m_stage->Release();
	}
}

void SpriteSelection::Clear()
{
	ObjSelectionSet<ISprite>::Clear();
	m_stage->SetCanvasDirty();
}

void SpriteSelection::Add(ISprite* item)
{
	ObjSelectionSet<ISprite>::Add(item);
	m_stage->SetCanvasDirty();
}

void SpriteSelection::Remove(ISprite* item)
{
	ObjSelectionSet<ISprite>::Remove(item);
	m_stage->SetCanvasDirty();
}

}