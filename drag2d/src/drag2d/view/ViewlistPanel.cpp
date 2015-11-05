#include "ViewlistPanel.h"
#include "ViewlistList.h"

#include "dataset/ISprite.h"
#include "dataset/ISymbol.h"
#include "widgets/VerticalImageList.h"
#include "view/SpritePropertySetting.h"
#include "view/MultiSpritesImpl.h"

#include "message/subject_id.h"
#include "message/SelectSpriteSJ.h"
#include "message/ReorderSpriteSJ.h"
#include "message/InsertSpriteSJ.h"
#include "message/RemoveSpriteSJ.h"
#include "message/ClearSpriteSJ.h"

#include <fstream>

namespace d2d
{

ViewlistPanel::ViewlistPanel(wxWindow* parent, EditPanelImpl* stage,
							 MultiSpritesImpl* sprites_impl /*= NULL*/)
	: wxPanel(parent, wxID_ANY)
	, m_stage(stage)
	, m_sprites_impl(sprites_impl)
	, m_selected_spr(NULL)
{
	InitLayout();

	m_subjects.push_back(SelectSpriteSJ::Instance());
	m_subjects.push_back(ReorderSpriteSJ::Instance());
	m_subjects.push_back(InsertSpriteSJ::Instance());
	m_subjects.push_back(RemoveSpriteSJ::Instance());
	m_subjects.push_back(ClearSpriteSJ::Instance());
	for (int i = 0; i < m_subjects.size(); ++i) {
		m_subjects[i]->Register(this);
	}
}

ViewlistPanel::~ViewlistPanel()
{
	if (m_selected_spr) {
		m_selected_spr->Release();
	}

	for (int i = 0; i < m_subjects.size(); ++i) {
		m_subjects[i]->UnRegister(this);
	}
}

void ViewlistPanel::Notify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case MSG_SELECT_SPRITE:
		{
			SelectSpriteSJ::Params* p = (SelectSpriteSJ::Params*)ud;
			Select(p->spr, p->clear);
		}
		break;
	case MSG_REORDER_SPRITE:
		{
			ReorderSpriteSJ::Params* p = (ReorderSpriteSJ::Params*)ud;
			Reorder(p->spr, p->up);
		}
		break;
	case MSG_INSERT_SPRITE:
		{
			InsertSpriteSJ::Params* p = (InsertSpriteSJ::Params*)ud;
			Insert(p->spr, p->idx);
		}
		break;
	case MSG_REMOVE_SPRITE:
		Remove((ISprite*)ud);
		break;
	case MSG_CLEAR_SPRITE:
		Clear();
		break;
	}
}

void ViewlistPanel::Insert(ISprite* sprite, int idx)
{
	ListItem* item = const_cast<ISymbol*>(&sprite->GetSymbol());
	if (idx < 0 || idx >= m_sprites.size()) {
		m_list->Insert(item, 0);
		m_sprites.insert(m_sprites.begin(), sprite);
	} else {
		int order = m_sprites.size() - idx;
		m_list->Insert(item, order);
		m_sprites.insert(m_sprites.begin() + order, sprite);
	}
}

void ViewlistPanel::RemoveSelected()
{
	int idx = m_list->GetSelection();
	m_list->Remove(idx);

	RemoveSpriteSJ::Instance()->Remove(m_sprites[idx], this);

	m_sprites.erase(m_sprites.begin() + idx);
}

void ViewlistPanel::ReorderSelected(bool up)
{
	if (!m_selected_spr) {
		return;
	}

	Reorder(m_selected_spr, up);

	ReorderSpriteSJ::Instance()->Reorder(m_selected_spr, up, this);
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

	bool add = m_list->GetKeyState(WXK_CONTROL);
	SelectSpriteSJ::Instance()->Select(spr, !add);
}

int ViewlistPanel::GetSelectedIndex() const
{
	return m_list->GetItemCount() - 1 - m_list->GetSelection();
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

void ViewlistPanel::Select(ISprite* spr, bool clear)
{
	int idx = QuerySprIdx(spr);
	if (idx >= 0) {
		m_list->SetSelection(idx);
	}
}

void ViewlistPanel::Reorder(const ISprite* sprite, bool up)
{
	int i = QuerySprIdx(sprite);
	if (i < 0) {
		return;
	}

	int n = m_sprites.size();
	if (up)
	{
		int pos = i - 1;
		if (pos >= 0)
		{
			std::swap(m_sprites[i], m_sprites[pos]);
			m_list->Swap(i, pos);
			m_list->SetSelection(pos);
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
		}
	}
}

void ViewlistPanel::Remove(ISprite* sprite)
{
	int idx = QuerySprIdx(sprite);
	if (idx < 0) {
		return;
	}
	m_list->Remove(idx);
	m_sprites.erase(m_sprites.begin() + idx);
}

void ViewlistPanel::Clear()
{
	m_list->Clear();
	m_sprites.clear();
}

} // d2d