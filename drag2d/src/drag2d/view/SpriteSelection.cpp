#include "SpriteSelection.h"
#include "IStageCanvas.h"
#include "message/SetCanvasDirtySJ.h"

namespace d2d
{

void SpriteSelection::Clear()
{
	ObjSelectionSet<Sprite>::Clear();
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void SpriteSelection::Add(Sprite* item)
{
	ObjSelectionSet<Sprite>::Add(item);
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void SpriteSelection::Remove(Sprite* item)
{
	ObjSelectionSet<Sprite>::Remove(item);
	SetCanvasDirtySJ::Instance()->SetDirty();
}

}