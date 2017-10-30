#include "EditableSpriteAOP.h"
#include "Sprite.h"
#include "panel_msg.h"

namespace ee
{

EditableSpriteAOP::EditableSpriteAOP(const std::vector<SprPtr>& sprs)
	: m_sprs(sprs)
{
}

void EditableSpriteAOP::Undo()
{
	for (auto& spr : m_sprs) {
		spr->SetEditable(!spr->IsVisible());
	}
	RefreshPanelSJ::Instance()->Refresh();
}

void EditableSpriteAOP::Redo()
{
	for (auto& spr : m_sprs) {
		spr->SetEditable(!spr->IsVisible());
	}
	RefreshPanelSJ::Instance()->Refresh();
}

void EditableSpriteAOP::Copy(const std::vector<SprPtr>& sprs)
{
	for (size_t i = 0, n = sprs.size(); i < n; ++i) {
		auto& spr = sprs[i];
		spr->SetEditable(!spr->IsVisible());
	}	
	RefreshPanelSJ::Instance()->Refresh();
}

Json::Value EditableSpriteAOP::Store(const std::vector<SprPtr>& sprs) const
{
	Json::Value val;
	return val;
}

}