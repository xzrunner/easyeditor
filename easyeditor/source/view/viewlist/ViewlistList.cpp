#include "ViewlistList.h"
#include "ViewlistListImpl.h"
#include "SettingData.h"
#include "Sprite.h"
#include "Symbol.h"
#include "subject_id.h"
#include "sprite_msg.h"
#include "Config.h"
#include "panel_msg.h"

#include <algorithm>

namespace ee
{

ViewlistList::ViewlistList(wxWindow* parent)
	: VerticalImageList(parent, "viewlist", false, Config::Instance()->GetSettings().img_list_compact)
	, m_impl(NULL)
	, m_selected_spr(NULL)
{
	RegistSubject(SelectSpriteSJ::Instance());
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
	if (m_impl) {
		m_impl->OnSelected(this, event.GetInt());
	} else {
		OnSelected(event.GetInt());
	}
}

void ViewlistList::OnListDoubleClicked(wxCommandEvent& event)
{
	if (m_impl) {
		m_impl->OnDoubleClicked(this, event.GetInt());
	} else {
		OnSelected(event.GetInt());
	}
}

void ViewlistList::SetImpl(ViewlistListImpl* impl)
{
	if (m_impl) {
		delete m_impl;
	}
	m_impl = impl;
}

void ViewlistList::OnSelected(int idx)
{
	if (Sprite* spr = QuerySprite(idx)) {
		OnSelected(spr);
	}
}

Sprite* ViewlistList::QuerySprite(int idx)
{
	Sprite* spr = NULL;
	if (idx >= 0 && idx < static_cast<int>(m_sprites.size())) {
		spr = m_sprites[idx];
	}
	return spr;
}

void ViewlistList::Clear()
{
	VerticalImageList::Clear();

	if (m_selected_spr) {
		m_selected_spr->Release();
		m_selected_spr = NULL;
	}

	for_each(m_sprites.begin(), m_sprites.end(), ReleaseObjectFunctor<Sprite>());
	m_sprites.clear();
}

void ViewlistList::Insert(Sprite* sprite, int idx)
{
	if (!sprite) {
		return;
	}
	sprite->Retain();

	ListItem* item = const_cast<Symbol*>(&sprite->GetSymbol());
	if (idx < 0 || idx >= static_cast<int>(m_sprites.size())) {
		VerticalImageList::Insert(item, 0);
		m_sprites.insert(m_sprites.begin(), sprite);
	} else {
		int order = m_sprites.size() - idx;
		VerticalImageList::Insert(item, order);
		m_sprites.insert(m_sprites.begin() + order, sprite);
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
		Remove((Sprite*)ud);
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

	static const int SPACE = 5, EDGE = 20;

	Sprite* spr = m_sprites[n];

	dc.SetBrush(spr->visiable ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
	dc.DrawRectangle(rect.x + SPACE, rect.y + SPACE, EDGE, EDGE);

	dc.SetBrush(spr->editable ? *wxBLACK_BRUSH : *wxWHITE_BRUSH);
	dc.DrawRectangle(rect.x + rect.width - SPACE - EDGE, rect.y + SPACE, EDGE, EDGE);
}

void ViewlistList::OnKeyDown(wxKeyEvent& event)
{
	int curr_idx = this->GetSelection();
	VerticalImageList::OnKeyDown(event);

	switch (event.GetKeyCode())
	{
	case WXK_PAGEUP:
		SetSelection(curr_idx);
		OnSelected(curr_idx);
		ReorderSelected(true);
		break;
	case WXK_PAGEDOWN:
		SetSelection(curr_idx);
		OnSelected(curr_idx);
		ReorderSelected(false);
		break;
	case WXK_DELETE:
		RemoveSelected();
		break;
	}

	if (m_selected_spr && GetKeyState(WXK_SHIFT))
	{
		int keycode = event.GetKeyCode();
		if (keycode == 'e' || keycode == 'E') {
			m_selected_spr->editable = !m_selected_spr->editable;
			Refresh();
		} else if (keycode == 's' || keycode == 'S') {
			m_selected_spr->visiable = !m_selected_spr->visiable;
			SetCanvasDirtySJ::Instance()->SetDirty();
			Refresh();
		}
	}
}

void ViewlistList::OnMouseEvent(wxMouseEvent& event)
{
	if (!event.LeftDown()) {
		return;
	}

	// todo 
	wxPoint pos = event.GetPosition();
}

int ViewlistList::GetSelectedIndex() const
{
	return GetItemCount() - 1 - GetSelection();
}

void ViewlistList::OnSelected(Sprite* spr)
{
	m_selected_spr = spr;
	m_selected_spr->Retain();

	bool add = GetKeyState(WXK_CONTROL);
	SelectSpriteSJ::Instance()->Select(spr, !add);
}

int ViewlistList::QuerySprIdx(const Sprite* spr) const
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		if (m_sprites[i] == spr) {
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

void ViewlistList::Select(Sprite* spr, bool clear)
{
	int idx = QuerySprIdx(spr);
	if (idx >= 0) {
		SetSelection(idx);
	}
}

void ViewlistList::Reorder(const Sprite* sprite, bool up)
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
			Swap(i, pos);
			SetSelection(pos);
		}
	}
	else
	{
		int pos = i + 1;
		if (pos < n)
		{
			std::swap(m_sprites[i], m_sprites[pos]);
			Swap(i, pos);
			SetSelection(pos);
		}
	}
}

void ViewlistList::ReorderMost(const Sprite* sprite, bool up)
{
	int i = QuerySprIdx(sprite);
	if (i < 0) {
		return;
	}

	ListItem* item = const_cast<Symbol*>(&sprite->GetSymbol());
	if (up) {
		if (i != 0) {
			m_sprites.erase(m_sprites.begin() + i);
			m_sprites.insert(m_sprites.begin(), const_cast<Sprite*>(sprite));

			VerticalImageList::Insert(item, 0);
			VerticalImageList::Remove(i + 1);

			SetSelection(0);
		}
	} else {
		if (i != m_sprites.size() - 1) {
			m_sprites.erase(m_sprites.begin() + i);
			m_sprites.push_back(const_cast<Sprite*>(sprite));

			VerticalImageList::Remove(i);
			VerticalImageList::Insert(item);

			SetSelection(m_sprites.size() - 1);
		}
	}
}

void ViewlistList::Remove(Sprite* sprite)
{
	int idx = QuerySprIdx(sprite);
	if (idx < 0) {
		return;
	}
	VerticalImageList::Remove(idx);

	sprite->Release();
	m_sprites.erase(m_sprites.begin() + idx);
}

void ViewlistList::RemoveSelected()
{
	int idx = GetSelection();
	VerticalImageList::Remove(idx);

	RemoveSpriteSJ::Instance()->Remove(m_sprites[idx], this);

	m_sprites[idx]->Release();
	m_sprites.erase(m_sprites.begin() + idx);
}

}