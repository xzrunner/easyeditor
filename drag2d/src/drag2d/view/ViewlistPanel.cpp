#include "ViewlistPanel.h"
#include "ViewlistList.h"

#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "widgets/VerticalImageList.h"
#include "view/EditPanel.h"
#include "view/SpritePropertySetting.h"
#include "view/MultiSpritesImpl.h"
#include "view/ViewPanelMgr.h"

#include <fstream>

namespace d2d
{

ViewlistPanel::ViewlistPanel(wxWindow* parent, EditPanel* stage,
							 MultiSpritesImpl* sprites_impl /*= NULL*/, 
							 ViewPanelMgr* view_panel_mgr /*= NULL*/)
	: wxPanel(parent, wxID_ANY)
	, m_stage(stage)
	, m_sprites_impl(sprites_impl)
	, m_view_panel_mgr(view_panel_mgr)
	, m_selected_spr(NULL)
{
	InitLayout();
}

ViewlistPanel::~ViewlistPanel()
{
	if (m_selected_spr) {
		m_selected_spr->Release();
	}
}

void ViewlistPanel::SelectSprite(ISprite* spr, bool clear)
{
	int idx = QuerySprIdx(spr);
	if (idx >= 0) {
		m_list->SetSelection(idx);
	}	
}

void ViewlistPanel::SelectMultiSprites(SpriteSelection* selection)
{
}

bool ViewlistPanel::ReorderSprite(ISprite* spr, bool up)
{
	return Reorder(spr, up);
}

bool ViewlistPanel::InsertSprite(ISprite* spr)
{
	return Insert(spr);
}

bool ViewlistPanel::RemoveSprite(ISprite* spr)
{
	return Remove(spr);
}

bool ViewlistPanel::Remove(ISprite* sprite)
{
	int idx = QuerySprIdx(sprite);
	if (idx < 0) {
		return false;
	}
	m_list->Remove(idx);
	m_sprites.erase(m_sprites.begin() + idx);
	return true;
}

bool ViewlistPanel::Insert(ISprite* sprite)
{
//	m_list->insert(const_cast<ISymbol*>(&sprite->getSymbol()));
//  m_sprites.push_back(sprite);

	m_list->InsertFront(const_cast<ISymbol*>(&sprite->GetSymbol()));
	m_sprites.insert(m_sprites.begin(), sprite);
	return true;
}

bool ViewlistPanel::Reorder(const ISprite* sprite, bool up)
{
	bool ret = false;

	int i = QuerySprIdx(sprite);
	assert(i >= 0);

	int n = m_sprites.size();
	if (up)
	{
		int pos = i - 1;
		if (pos >= 0)
		{
			std::swap(m_sprites[i], m_sprites[pos]);
			m_list->Swap(i, pos);
			m_list->SetSelection(pos);
			ret = true;
		}
	}
	else
	{
		int pos = i + 1;
		if (pos < n)
		{
			std::swap(m_sprites[i], m_sprites[pos]);
			m_list->Swap(i, pos);
			m_list->SetSelection(pos);
			ret = true;
		}
	}

	return ret;
}

bool ViewlistPanel::ReorderSelected(bool up)
{
	bool ret = false;

	if (!m_selected_spr) {
		return ret;
	}

	ret = Reorder(m_selected_spr, up);
	if (m_view_panel_mgr) {
		m_view_panel_mgr->ReorderSprite(m_selected_spr, up, this);
	}

	return ret;
}

void ViewlistPanel::OnSelected(int index)
{
	d2d::ISprite* spr = NULL;
	if (index >= 0 && index < m_sprites.size()) {
		spr = m_sprites[index];
	}
	if (spr) {
		OnSelected(spr);
	}
}

void ViewlistPanel::OnSelected(d2d::ISprite* spr)
{
	m_selected_spr = spr;
	m_selected_spr->Retain();

	if (m_view_panel_mgr) {
		bool add = m_stage->GetKeyState(WXK_CONTROL);
		m_view_panel_mgr->SelectSprite(spr, !add, this);
	}
}

void ViewlistPanel::Clear()
{
	m_list->Clear();
	m_sprites.clear();
}

void ViewlistPanel::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	// title
	wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT(" View List"));
	title->SetForegroundColour(*wxWHITE);
	title->SetBackgroundColour(*wxBLACK);
	sizer->Add(title, 0, wxEXPAND);

	m_list = new ViewlistList(this);
	sizer->Add(m_list, 1, wxEXPAND);

	SetSizer(sizer);
}

int ViewlistPanel::QuerySprIdx(const ISprite* spr) const
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		if (m_sprites[i] == spr) {
			return i;
		}
	}
	return -1;
}

} // d2d