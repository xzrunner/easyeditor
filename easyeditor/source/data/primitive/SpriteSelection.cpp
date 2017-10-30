#include "SpriteSelection.h"
#include "panel_msg.h"

namespace ee
{

void SpriteSelection::Clear()
{
	SelectionSet<Sprite>::Clear();
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void SpriteSelection::Add(const SprPtr& item)
{
	SelectionSet<Sprite>::Add(item);
	SetCanvasDirtySJ::Instance()->SetDirty();
}

void SpriteSelection::Remove(const SprPtr& item)
{
	SelectionSet<Sprite>::Remove(item);
	SetCanvasDirtySJ::Instance()->SetDirty();
}

}