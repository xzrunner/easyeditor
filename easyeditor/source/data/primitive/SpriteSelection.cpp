#include "SpriteSelection.h"
#include "panel_msg.h"

namespace ee
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