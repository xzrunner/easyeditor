#include "VisibleSpriteAOP.h"
#include "Sprite.h"
#include "panel_msg.h"

namespace ee
{

VisibleSpriteAOP::VisibleSpriteAOP(const std::vector<SprPtr>& sprs)
	: m_sprs(sprs)
{
}

void VisibleSpriteAOP::Undo()
{
	for (auto& spr : m_sprs) {
		spr->SetVisible(!spr->IsVisible());
	}
	SetCanvasDirtySJ::Instance()->SetDirty();
	RefreshPanelSJ::Instance()->Refresh();
}

void VisibleSpriteAOP::Redo()
{
	for (auto& spr : m_sprs) {
		spr->SetVisible(!spr->IsVisible());
	}
	SetCanvasDirtySJ::Instance()->SetDirty();
	RefreshPanelSJ::Instance()->Refresh();
}

void VisibleSpriteAOP::Copy(const std::vector<SprPtr>& sprs)
{
	for (auto& spr : sprs) {
		spr->SetVisible(!spr->IsVisible());
	}	
	SetCanvasDirtySJ::Instance()->SetDirty();
	RefreshPanelSJ::Instance()->Refresh();
}

Json::Value VisibleSpriteAOP::Store(const std::vector<SprPtr>& sprs) const
{
	Json::Value val;
	return val;
}

}