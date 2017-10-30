#include "ViewlistList.h"
#include "ViewlistListImpl.h"
#include "SettingData.h"
#include "Sprite.h"
#include "Symbol.h"
#include "subject_id.h"
#include "sprite_msg.h"
#include "Config.h"
#include "panel_msg.h"
#include "SpriteSelection.h"
#include "FetchAllVisitor.h"
#include "DeleteSpriteAOP.h"

#include <algorithm>

namespace ee
{

static const int BTN_SPACE = 5, BTN_EDGE = 20;

ViewlistList::ViewlistList(wxWindow* parent)
	: VerticalImageList(parent, "viewlist", false, Config::Instance()->GetSettings().img_list_compact, true)
	, m_impl(NULL)
	, m_selected_spr(NULL)
{
	RegistSubject(SelectSpriteSJ::Instance());
	RegistSubject(SelectSpriteSetSJ::Instance());
	RegistSubject(ClearSpriteSelectionSJ::Instance());
	RegistSubject(ReorderSpriteSJ::Instance());
	RegistSubject(ReorderSpriteMostSJ::Instance());
	RegistSubject(InsertSpriteSJ::Instance());
	RegistSubject(RemoveSpriteSJ::Instance());
	RegistSubject(ClearSpriteSJ::Instance());
	RegistSubject(QuerySelectedSpriteLayerSJ::Instance());
	RegistSubject(ClearPanelSJ::Instance());
	RegistSubject(RefreshPanelSJ::Instance());
}

ViewlistList::~ViewlistList()
{
	if (m_impl) {
		delete m_impl;
	}

	Clear();
}

void ViewlistList::OnListSelected(wxCommandEvent& event)
{
	std::vector<int> selected;
	selected.reserve(GetSelectedCount());
	unsigned long cookie;
	int item = GetFirstSelected(cookie);
	while (item != wxNOT_FOUND) {
		selected.push_back(item);
		item = GetNextSelected(cookie);
	}

	ClearSpriteSelectionSJ::Instance()->Clear();

	bool clear = selected.size() == 1;
	for (int i = selected.size() - 1; i >= 0; --i)
	{
		if (m_impl) {
			m_impl->OnSelected(this, selected[i], clear);
		} else {
			OnSelected(selected[i], clear);
		}
	}
}

void ViewlistList::OnListDoubleClicked(wxCommandEvent& event)
{
	if (m_impl) {
		m_impl->OnDoubleClicked(this, event.GetInt());
	} else {
		OnSelected(event.GetInt(), true);
	}
}

void ViewlistList::SetImpl(ViewlistListImpl* impl)
{
	if (m_impl) {
		delete m_impl;
	}
	m_impl = impl;
}

void ViewlistList::OnSelected(int idx, bool clear)
{
	if (auto spr = QuerySprite(idx)) {
		OnSelected(spr, clear);
	}
}

SprPtr ViewlistList::QuerySprite(int idx)
{
	SprPtr ret = nullptr;
	if (idx >= 0 && idx < static_cast<int>(m_sprs.size())) {
		ret = m_sprs[idx];
	}
	return ret;
}

void ViewlistList::Clear()
{
	VerticalImageList::Clear();

	m_selected_spr.reset();
	m_sprs.clear();
}

void ViewlistList::Insert(const SprPtr& spr, int idx)
{
	if (!spr) {
		return;
	}

	auto item = std::dynamic_pointer_cast<ListItem>(spr->GetSymbol());
	if (idx < 0 || idx >= static_cast<int>(m_sprs.size())) {
		VerticalImageList::Insert(item, 0);
		m_sprs.insert(m_sprs.begin(), spr);
	} else {
		int order = m_sprs.size() - idx;
		VerticalImageList::Insert(item, order);
		m_sprs.insert(m_sprs.begin() + order, spr);
	}
}

void ViewlistList::OnNotify(int sj_id, void* ud)
{
	switch (sj_id)
	{
	case MSG_SELECT_SPRITE:
		{
			SelectSpriteSJ::Params* p = (SelectSpriteSJ::Params*)ud;
			Select(p->spr, p->clear);
		}
		break;
	case MSG_SELECT_SPRITE_SET:
		{
			SpriteSelection* selection = (SpriteSelection*)ud;
			SelectSet(selection);
		}
		break;
	case MSG_CLEAR_SPRITE_SELECTION:
		{
			SetSelection(-1);
		}
		break;
	case MSG_REORDER_SPRITE:
		{
			ReorderSpriteSJ::Params* p = (ReorderSpriteSJ::Params*)ud;
			Reorder(p->spr, p->up);
		}
		break;
	case MSG_REORDER_SPRITE_MOST:
		{
			ReorderSpriteMostSJ::Params* p = (ReorderSpriteMostSJ::Params*)ud;
			ReorderMost(p->spr, p->up);
		}
		break;
	case MSG_INSERT_SPRITE:
		{
			InsertSpriteSJ::Params* p = (InsertSpriteSJ::Params*)ud;
			Insert(p->spr, p->idx);
		}
		break;
	case MSG_REMOVE_SPRITE:
		Remove(*(SprPtr*)ud);
		break;
	case MSG_CLEAR_SPRITE: case MSG_CLEAR_PANEL:
		Clear();
		break;
	case MSG_QUERY_SPR_LAYER:
		{
			int* layer = (int*)ud;
			*layer = GetSelectedIndex();
		}
		break;
	case MSG_REFRESH_PANEL:
		Refresh();
		break;
	}
}

void ViewlistList::OnDrawItem(wxDC& dc, const wxRect& rect, size_t n) const
{
	VerticalImageList::OnDrawItem(dc, rect, n);

	const SprPtr& spr = m_sprs[n];

	dc.SetBrush(spr->IsVisible() ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
	dc.DrawRectangle(rect.x + BTN_SPACE, rect.y + BTN_SPACE, BTN_EDGE, BTN_EDGE);

	dc.SetBrush(spr->IsEditable() ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
	dc.DrawRectangle(rect.x + rect.width - BTN_SPACE - BTN_EDGE, rect.y + BTN_SPACE, BTN_EDGE, BTN_EDGE);
}

void ViewlistList::OnKeyDown(wxKeyEvent& event)
{
	int curr_idx;
	if (HasMultipleSelection()) {
		unsigned long cookie;
		curr_idx = GetFirstSelected(cookie);
	} else {
		curr_idx = this->GetSelection();
	}
	VerticalImageList::OnKeyDown(event);

	switch (event.GetKeyCode())
	{
	case WXK_PAGEUP:
		OnSelected(curr_idx, true);
		ReorderSelected(true);
		break;
	case WXK_PAGEDOWN:
		OnSelected(curr_idx, true);
		ReorderSelected(false);
		break;
	case WXK_DELETE:
		RemoveSelected();
		break;
	}

	if (m_selected_spr && GetKeyState(WXK_SHIFT))
	{
		int keycode = event.GetKeyCode();
		if (keycode == 'E') {
			m_selected_spr->SetEditable(!m_selected_spr->IsEditable());
			Refresh();
		} else if (keycode == 'S') {
			m_selected_spr->SetVisible(!m_selected_spr->IsVisible());
			SetCanvasDirtySJ::Instance()->SetDirty();
			Refresh();
		}
	}
}

void ViewlistList::OnMouse(wxMouseEvent& event)
{
	if (!event.LeftDown()) {
		event.Skip();
		return;
	}

	wxPoint pos = event.GetPosition();
	int w = GetSize().GetWidth();
	if (pos.x > BTN_SPACE + BTN_EDGE && pos.x < w - 25 - BTN_SPACE - BTN_EDGE) {
		return;
	}
	int curr_pos = HitTest(pos);
	if (pos.y > BTN_SPACE + BTN_EDGE) {
		int up_pos = HitTest(pos.x, pos.y - BTN_SPACE - BTN_EDGE);
		if (curr_pos == up_pos) {
			return;
		}
	}

	const SprPtr& spr = m_sprs[curr_pos];
	if (pos.x <= BTN_SPACE + BTN_EDGE) {
		spr->SetVisible(!spr->IsVisible());
	} else {
		spr->SetEditable(!spr->IsEditable());
	}
	RefreshPanelSJ::Instance()->Refresh();
}

int ViewlistList::GetSelectedIndex() const
{
	int selected;
	if (HasMultipleSelection()) {
		unsigned long cookie;
		selected = GetFirstSelected(cookie);
	} else {
		selected = GetSelection();
	}
	return GetItemCount() - 1 - selected;
}

void ViewlistList::OnSelected(const SprPtr& spr, bool clear)
{
	m_selected_spr = spr;
	SelectSpriteSJ::Instance()->Select(spr, clear);
}

int ViewlistList::QuerySprIdx(const SprConstPtr& spr) const
{
	for (int i = 0, n = m_sprs.size(); i < n; ++i) {
		if (m_sprs[i] == spr) {
			return i;
		}
	}
	return -1;
}

void ViewlistList::ReorderSelected(bool up)
{
	if (!m_selected_spr) {
		return;
	}

	Reorder(m_selected_spr, up);

	ReorderSpriteSJ::Instance()->Reorder(m_selected_spr, up, this);
}

void ViewlistList::Select(const SprPtr& spr, bool clear)
{
	if (clear) {
		SetSelection(-1);
	}
	int idx = QuerySprIdx(spr);
	if (idx >= 0) {
		SetSelection(idx);
	}
}

void ViewlistList::SelectSet(SpriteSelection* set)
{
	SetSelection(-1);
	std::vector<SprPtr> sprs;
	set->Traverse(FetchAllRefVisitor<Sprite>(sprs));
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		int idx = QuerySprIdx(sprs[i]);
		if (idx >= 0) {
			VerticalImageList::Select(idx, true);
		}
	}
}

void ViewlistList::Reorder(const SprConstPtr& spr, bool up)
{
	int i = QuerySprIdx(spr);
	if (i < 0) {
		return;
	}

	int n = m_sprs.size();
	if (up)
	{
		int pos = i - 1;
		if (pos >= 0)
		{
			std::swap(m_sprs[i], m_sprs[pos]);
			Swap(i, pos);
			SetSelection(-1);
			SetSelection(pos);
		}
	}
	else
	{
		int pos = i + 1;
		if (pos < n)
		{
			std::swap(m_sprs[i], m_sprs[pos]);
			Swap(i, pos);
			SetSelection(-1);
			SetSelection(pos);
		}
	}
}

void ViewlistList::ReorderMost(const SprConstPtr& spr, bool up)
{
	int i = QuerySprIdx(spr);
	if (i < 0) {
		return;
	}

	auto item = std::dynamic_pointer_cast<ListItem>(spr->GetSymbol());
	if (up) {
		if (i != 0) {
			m_sprs.erase(m_sprs.begin() + i);
			m_sprs.insert(m_sprs.begin(), std::const_pointer_cast<Sprite>(std::dynamic_pointer_cast<const Sprite>(spr)));

			VerticalImageList::Insert(item, 0);
			VerticalImageList::Remove(i + 1);

			SetSelection(-1);
			SetSelection(0);
		}
	} else {
		if (i != m_sprs.size() - 1) {
			m_sprs.erase(m_sprs.begin() + i);
			m_sprs.push_back(std::const_pointer_cast<Sprite>(std::dynamic_pointer_cast<const Sprite>(spr)));

			VerticalImageList::Remove(i);
			VerticalImageList::Insert(item);

			SetSelection(-1);
			SetSelection(m_sprs.size() - 1);
		}
	}
}

void ViewlistList::Remove(const SprPtr& spr)
{
	int idx = QuerySprIdx(spr);
	if (idx < 0) {
		return;
	}
	VerticalImageList::Remove(idx);

	m_sprs.erase(m_sprs.begin() + idx);
}

void ViewlistList::RemoveSelected()
{
	int selected;
	if (HasMultipleSelection()) {
		unsigned long cookie;
		selected = GetFirstSelected(cookie);
	} else {
		selected = GetSelection();
	}

	// add to history
	std::vector<SprPtr> sprs;
	sprs.push_back(m_sprs[selected]);
	EditAddRecordSJ::Instance()->Add(new DeleteSpriteAOP(sprs));

	VerticalImageList::Remove(selected);

	RemoveSpriteSJ::Instance()->Remove(m_sprs[selected], this);

	m_sprs.erase(m_sprs.begin() + selected);
}

}