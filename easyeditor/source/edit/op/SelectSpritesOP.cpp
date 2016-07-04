#include "SelectSpritesOP.h"
#include "FontBlankSprite.h"
#include "SymbolMgr.h"
#include "SpriteFactory.h"
#include "EditCMPT.h"
#include "MultiSpritesImpl.h"
#include "StageCanvas.h"
#include "SpriteSelection.h"
#include "EditPanelImpl.h"
#include "DrawSelectedSpriteVisitor.h"
#include "EE_RVG.h"
#include "sprite_msg.h"
#include "panel_msg.h"
#include "FetchAllVisitor.h"
#include "PointMultiQueryVisitor.h"

#include <wx/clipbrd.h>

#include <sstream>

namespace ee
{

SelectSpritesOP::SelectSpritesOP(wxWindow* wnd, EditPanelImpl* stage, 
								 MultiSpritesImpl* sprites_impl, 
								 EditCMPT* callback)
	: DrawSelectRectOP(wnd, stage)
	, m_callback(callback)
	, m_spritesImpl(sprites_impl)
	, m_draggable(true)
	, m_rect_select(false)
{
	m_left_first_pos.MakeInvalid();

	m_selection = sprites_impl->GetSpriteSelection();
	m_selection->Retain();
}

SelectSpritesOP::~SelectSpritesOP()
{
	m_selection->Clear();
	ClearSpriteSelectionSJ::Instance()->Clear();
	m_selection->Release();
}

bool SelectSpritesOP::OnKeyDown(int keyCode)
{
	if (DrawSelectRectOP::OnKeyDown(keyCode)) return true;

	if (m_stage->GetKeyState(WXK_CONTROL) && m_stage->GetKeyState(WXK_CONTROL_X))
	{
		PasteToSelection();
		m_spritesImpl->ClearSelectedSprite();
		return true;
	}
	else if (m_stage->GetKeyState(WXK_CONTROL) && keyCode == 'C')
	{
		PasteToSelection();
		return true;
	}
	else if (m_stage->GetKeyState(WXK_CONTROL) && keyCode == 'V')
	{
		CopyFromSelection();
		return true;
	}
	else if (keyCode ==  WXK_SHIFT)
	{
		if (!m_rect_select) {
			m_rect_select = true;
			m_stage->SetCursor(wxCURSOR_RIGHT_ARROW);
		}
	}

	return false;
}

bool SelectSpritesOP::OnKeyUp(int keyCode)
{
	if (DrawSelectRectOP::OnKeyUp(keyCode)) return true;

	switch (keyCode)
	{
	case WXK_SHIFT:
		if (m_rect_select) {
			m_rect_select = false;
			m_stage->SetCursor(wxCURSOR_ARROW);
		}
		break;
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftDown(int x, int y)
{
 	m_draggable = true;

	sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
	Sprite* selected = NULL;
	if (!m_rect_select) {
		selected = SelectByPos(pos);
	}
	if (selected)
	{
		assert(selected->IsEditable());
		if (m_stage->GetKeyState(WXK_CONTROL))
		{
			if (m_selection->IsExist(selected)) {
				m_selection->Remove(selected);
			} else {
				m_selection->Add(selected);
			}
			SelectSpriteSetSJ::Instance()->Select(m_selection, m_spritesImpl);
		}
		else
		{
			if (!m_selection->IsExist(selected) && !m_stage->GetKeyState(WXK_SPACE))
			{
				m_selection->Clear();
				ClearSpriteSelectionSJ::Instance()->Clear();
				m_selection->Add(selected);
				SelectSpriteSJ::Instance()->Select(selected, true);
			}
		}
		m_left_first_pos.MakeInvalid();

		if (m_callback)
			m_callback->UpdateControlValue();
	}
	else
	{
		DrawSelectRectOP::OnMouseLeftDown(x, y);

		if (m_stage->GetKeyState(WXK_SPACE)) {
			m_left_first_pos.MakeInvalid();
		} else {
			m_left_first_pos = pos;
		}

		if (m_stage->GetKeyState(WXK_CONTROL)) {
			m_draggable = false;
		} else {
			m_selection->Clear();
			ClearSpriteSelectionSJ::Instance()->Clear();
		}
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftUp(int x, int y)
{
	if (DrawSelectRectOP::OnMouseLeftUp(x, y)) return true;

	m_draggable = true;

	if (!m_left_first_pos.IsValid()) {
		return false;
	}

	sm::vec2 end = m_stage->TransPosScrToProj(x, y);
	sm::rect rect(m_left_first_pos, end);
	std::vector<Sprite*> sprites;
	m_spritesImpl->QuerySpritesByRect(rect, m_left_first_pos.x < end.x, sprites);
	if (m_stage->GetKeyState(WXK_CONTROL))
	{
		for (size_t i = 0, n = sprites.size(); i < n; ++i) 
		{
			Sprite* sprite = sprites[i];
			if (m_selection->IsExist(sprite)) {
				m_selection->Remove(sprites[i]);
			} else {
				m_selection->Add(sprites[i]);
			}
		}
	}
	else
	{
		for (size_t i = 0, n = sprites.size(); i < n; ++i) {
			m_selection->Add(sprites[i]);
		}
	}

	SelectSpriteSetSJ::Instance()->Select(m_selection, m_spritesImpl);

	m_left_first_pos.MakeInvalid();

	if (m_callback) {
		m_callback->UpdateControlValue();
	}

	SetCanvasDirtySJ::Instance()->SetDirty();

	return false;
}

bool SelectSpritesOP::OnMouseRightDown(int x, int y)
{
	m_right_first_pos.Set(x, y);

	SetRightPan(m_selection->IsEmpty());
	if (DrawSelectRectOP::OnMouseRightDown(x, y)) return true;

	return false;
}

bool SelectSpritesOP::OnMouseRightUp(int x, int y)
{
	// select
	if (m_right_first_pos == sm::vec2(x, y))
	{
		sm::vec2 pos = m_stage->TransPosScrToProj(x, y);
		PointMultiQueryVisitor visitor(pos);
		m_spritesImpl->TraverseSprites(visitor, DT_EDITABLE);
		const std::vector<Sprite*>& sprites = visitor.GetResult();
		SetRightPan(sprites.empty());
	}

	if (DrawSelectRectOP::OnMouseRightUp(x, y)) return true;

	return false;
}

bool SelectSpritesOP::OnMouseDrag(int x, int y)
{
	if (DrawSelectRectOP::OnMouseDrag(x, y)) return true;

	return !m_draggable;
}

bool SelectSpritesOP::OnDraw() const
{
	if (DrawSelectRectOP::OnDraw()) {
		return true;
	}

	m_selection->Traverse(DrawSelectedSpriteVisitor(s2::Color(255, 0, 0)));

	return false;
}

bool SelectSpritesOP::Clear()
{
	if (DrawSelectRectOP::Clear()) return true;

	m_selection->Clear();
	ClearSpriteSelectionSJ::Instance()->Clear();

	m_left_first_pos.MakeInvalid();

	return false;
}

Sprite* SelectSpritesOP::SelectByPos(const sm::vec2& pos) const
{
	if (m_stage->GetKeyState(WXK_SPACE)) {
		return NULL;
	}

	Sprite* selected = NULL;
	std::vector<Sprite*> sprites;
	m_spritesImpl->GetSpriteSelection()->Traverse(FetchAllVisitor<Sprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		Sprite* spr = sprites[i];
		if (spr->IsEditable() && spr->IsContain(pos)) {
			selected = spr;
			break;
		}
	}
	if (!selected) {
		Sprite* spr = m_spritesImpl->QuerySpriteByPos(pos);
		if (spr) {
			selected = spr;
		}
	}
	return selected;
}

void SelectSpritesOP::PasteSprToClipboard(const Sprite* spr, Json::Value& value) const
{
	value["filename"] = spr->GetSymbol().GetFilepath();
	spr->Store(value);	
}

void SelectSpritesOP::CopySprFromClipboard(Sprite* spr, const Json::Value& value) const
{
	spr->Load(value);
}

void SelectSpritesOP::PasteToSelection() const
{
	std::vector<Sprite*> sprites;
	m_selection->Traverse(FetchAllVisitor<Sprite>(sprites));
	Json::Value value;
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		Json::Value& sval = value["sprite"][i];
		Sprite* s = sprites[i];
		if (wxTheClipboard->Open()) {
			PasteSprToClipboard(s, sval);
		}
	}
	Json::StyledStreamWriter writer;
	std::stringstream ss;
	writer.write(ss, value);
	wxTheClipboard->SetData(new wxTextDataObject(ss.str()));
	wxTheClipboard->Close();
}

void SelectSpritesOP::CopyFromSelection()
{
	if (!wxTheClipboard->Open()) {
		return;
	}

	if (!wxTheClipboard->IsSupported( wxDF_TEXT )) {
		wxTheClipboard->Close();
		return;
	}

	wxTextDataObject data;
	wxTheClipboard->GetData( data );

	Json::Value value;
	Json::Reader reader;
	std::string test = data.GetText().ToStdString();
	reader.parse(data.GetText().ToStdString(), value);

	if (value.isNull()) {
		wxTheClipboard->Close();
		return;
	}
 
	m_selection->Clear();
	ClearSpriteSelectionSJ::Instance()->Clear();

	Sprite* last_spr = NULL;

	std::vector<Sprite*> sprites;

	int i = 0;
	Json::Value sval = value["sprite"][i++];
	while (!sval.isNull()) {
		std::string filepath = sval["filename"].asString();
		// fixme
		if (filepath != "group") {
			Symbol* symbol = SymbolMgr::Instance()->FetchSymbol(filepath);
			// for snapshoot
			symbol->RefreshThumbnail(filepath);
			Sprite* sprite = SpriteFactory::Instance()->Create(symbol);
			sprites.push_back(sprite);
			symbol->Release();
			CopySprFromClipboard(sprite, sval);
			InsertSpriteSJ::Instance()->Insert(sprite);
			last_spr = sprite;
		}
		sval = value["sprite"][i++];
	}

	for (int i = 0, n = sprites.size(); i < n; ++i) {
		m_selection->Add(sprites[i]);
	}

	bool add = m_stage->GetKeyState(WXK_CONTROL);
	SelectSpriteSJ::Instance()->Select(last_spr, !add);

	wxTheClipboard->Close();
}

}