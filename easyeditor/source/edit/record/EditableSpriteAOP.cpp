#include "EditableSpriteAOP.h"
#include "Sprite.h"
#include "panel_msg.h"

namespace ee
{

EditableSpriteAOP::EditableSpriteAOP(const std::vector<Sprite*>& sprs)
{
	for (size_t i = 0, n = sprs.size(); i < n; ++i) 
	{
		sprs[i]->AddReference();
		m_sprs.push_back(sprs[i]);
	}
}

EditableSpriteAOP::~EditableSpriteAOP()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->RemoveReference();
	}
}

void EditableSpriteAOP::Undo()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		ee::Sprite* spr = m_sprs[i];
		spr->SetEditable(!spr->IsVisible());
	}
	RefreshPanelSJ::Instance()->Refresh();
}

void EditableSpriteAOP::Redo()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		ee::Sprite* spr = m_sprs[i];
		spr->SetEditable(!spr->IsVisible());
	}
	RefreshPanelSJ::Instance()->Refresh();
}

void EditableSpriteAOP::Redo(const std::vector<Sprite*>& sprs)
{
	for (size_t i = 0, n = sprs.size(); i < n; ++i) {
		ee::Sprite* spr = sprs[i];
		spr->SetEditable(!spr->IsVisible());
	}	
	RefreshPanelSJ::Instance()->Refresh();
}

Json::Value EditableSpriteAOP::Store(const std::vector<Sprite*>& sprs) const
{
	Json::Value val;
	return val;
}

}