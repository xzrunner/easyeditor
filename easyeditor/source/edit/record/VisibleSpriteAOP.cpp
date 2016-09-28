#include "VisibleSpriteAOP.h"
#include "Sprite.h"
#include "panel_msg.h"

namespace ee
{

VisibleSpriteAOP::VisibleSpriteAOP(const std::vector<Sprite*>& sprs)
{
	for (size_t i = 0, n = sprs.size(); i < n; ++i) 
	{
		sprs[i]->AddReference();
		m_sprs.push_back(sprs[i]);
	}
}

VisibleSpriteAOP::~VisibleSpriteAOP()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		m_sprs[i]->RemoveReference();
	}
}

void VisibleSpriteAOP::Undo()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		ee::Sprite* spr = m_sprs[i];
		spr->SetVisible(!spr->IsVisible());
	}
	SetCanvasDirtySJ::Instance()->SetDirty();
	RefreshPanelSJ::Instance()->Refresh();
}

void VisibleSpriteAOP::Redo()
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i) {
		ee::Sprite* spr = m_sprs[i];
		spr->SetVisible(!spr->IsVisible());
	}
	SetCanvasDirtySJ::Instance()->SetDirty();
	RefreshPanelSJ::Instance()->Refresh();
}

void VisibleSpriteAOP::Copy(const std::vector<Sprite*>& sprs)
{
	for (size_t i = 0, n = sprs.size(); i < n; ++i) {
		ee::Sprite* spr = sprs[i];
		spr->SetVisible(!spr->IsVisible());
	}	
	SetCanvasDirtySJ::Instance()->SetDirty();
	RefreshPanelSJ::Instance()->Refresh();
}

Json::Value VisibleSpriteAOP::Store(const std::vector<Sprite*>& sprs) const
{
	Json::Value val;
	return val;
}

}