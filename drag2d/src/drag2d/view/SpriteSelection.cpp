#include "SpriteSelection.h"
#include "IStageCanvas.h"

#include "view/EditPanel.h"

namespace d2d
{

SpriteSelection::SpriteSelection(EditPanel* stage)
	: m_stage(stage)
{
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