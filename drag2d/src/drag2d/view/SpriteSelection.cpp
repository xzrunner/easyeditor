#include "SpriteSelection.h"
#include "IStageCanvas.h"
#include "message/SetCanvasDirtySJ.h"

namespace d2d
{

void SpriteSelection::Clear()
{
	ObjSelectionSet<ISprite>::Clear();
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void SpriteSelection::Add(ISprite* item)
{
	ObjSelectionSet<ISprite>::Add(item);
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void SpriteSelection::Remove(ISprite* item)
{
	ObjSelectionSet<ISprite>::Remove(item);
	SetCanvasDirtySJ::Instance()->SetDirty();
}

}