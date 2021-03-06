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
#include "sprite_msg.h"
#include "panel_msg.h"
#include "FetchAllVisitor.h"
#include "PointMultiQueryVisitor.h"
#include "InsertSpriteAOP.h"
#include "Exception.h"
#include "ExceptionDlg.h"
#include "SymbolType.h"
#include "FileHelper.h"

#include <sprite2/RVG.h>

#include <wx/clipbrd.h>

#include <sstream>

namespace ee
{

SelectSpritesOP::SelectSpritesOP(wxWindow* wnd, EditPanelImpl* stage, 
								 MultiSpritesImpl* sprites_impl, 
								 EditCMPT* callback)
	: DrawSelectRectOP(wnd, stage)
	, m_callback(callback)
	, m_sprs_impl(sprites_impl)
	, m_draggable(true)
	, m_rect_select(false)
{
	m_left_first_pos.MakeInvalid();

	m_selection = sprites_impl->GetSpriteSelection();
	m_selection->AddReference();
}

SelectSpritesOP::~SelectSpritesOP()
{
	m_selection->Clear();
	ClearSpriteSelectionSJ::Instance()->Clear();
	m_selection->RemoveReference();
}

bool SelectSpritesOP::OnKeyDown(int keyCode)
{
	if (DrawSelectRectOP::OnKeyDown(keyCode)) return true;

	if (m_stage->GetKeyState(WXK_CONTROL) && m_stage->GetKeyState(WXK_CONTROL_X))
	{
		PasteToSelection();
		m_sprs_impl->ClearSelectedSprite();
		return true;
	}
	else if (m_stage->GetKeyState(WXK_CONTROL) && keyCode == 'C')
	{
		PasteToSelection();
		return true;
	}
	else if (m_stage->GetKeyState(WXK_CONTROL) && keyCode == 'V')
	{
		try {
			CopyFromSelection();
		} catch (ee::Exception& e) {
			ee::ExceptionDlg dlg(m_wnd, e);
			dlg.ShowModal();
		}
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
	SprPtr selected = nullptr;
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
			SelectSpriteSetSJ::Instance()->Select(m_selection, m_sprs_impl);
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
	std::vector<SprPtr> sprs;
	m_sprs_impl->QuerySpritesByRect(rect, m_left_first_pos.x < end.x, sprs);
	if (m_stage->GetKeyState(WXK_CONTROL))
	{
		for (auto& spr : sprs) 
		{
			if (m_selection->IsExist(spr)) {
				m_selection->Remove(spr);
			} else {
				m_selection->Add(spr);
			}
		}
	}
	else
	{
		for (size_t i = 0, n = sprs.size(); i < n; ++i) {
			m_selection->Add(sprs[i]);
		}
	}

	SelectSpriteSetSJ::Instance()->Select(m_selection, m_sprs_impl);

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
		m_sprs_impl->TraverseSprites(visitor, DT_EDITABLE);
		auto& sprs = visitor.GetResult();
		SetRightPan(sprs.empty());
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

	m_selection->Traverse(DrawSelectedSpriteVisitor(pt2::Color(255, 0, 0)));

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

SprPtr SelectSpritesOP::SelectByPos(const sm::vec2& pos) const
{
	if (m_stage->GetKeyState(WXK_SPACE)) {
		return nullptr;
	}

	SprPtr selected = nullptr;
	std::vector<SprPtr> sprs;
	m_sprs_impl->GetSpriteSelection()->Traverse(FetchAllRefVisitor<Sprite>(sprs));
	for (auto& spr : sprs) {
		if (spr->IsEditable() && spr->GetBounding().IsContain(pos)) {
			selected = spr;
			break;
		}
	}
	if (!selected) {
		auto spr = m_sprs_impl->QuerySpriteByPos(pos);
		if (spr) {
			selected = spr;
		}
	}
	return selected;
}

void SelectSpritesOP::PasteSprToClipboard(const SprConstPtr& spr, Json::Value& value) const
{
	auto filepath = std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol())->GetFilepath();
	value["filepath"] = filepath.c_str();
	auto dir = ee::FileHelper::GetFileDir(filepath);
	spr->Store(value, dir);	
}

void SelectSpritesOP::CopySprFromClipboard(const SprPtr& spr, const Json::Value& value) const
{
	auto dir = ee::FileHelper::GetFileDir(std::dynamic_pointer_cast<ee::Symbol>(spr->GetSymbol())->GetFilepath());
	spr->Load(value, dir);
}

void SelectSpritesOP::PasteToSelection() const
{
	std::vector<SprPtr> sprs;
	GetOrderedSelection(sprs);
	Json::Value value;
	for (int i = 0, n = sprs.size(); i < n; ++i)
	{
		Json::Value& sval = value["sprite"][i];
		auto& s = sprs[i];
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

	std::string str = data.GetText().ToStdString();
	if (str.find("{") == std::string::npos) {
		wxTheClipboard->Close();
		return;
	}

	Json::Value value;
	Json::Reader reader;
	std::string test = data.GetText().ToStdString();
	bool succ = reader.parse(str, value);
	if (!succ || value["sprite"].isNull()) {
 		wxTheClipboard->Close();
 		return;
 	}
  
	m_selection->Clear();
	ClearSpriteSelectionSJ::Instance()->Clear();

	std::vector<SprPtr> sprs;

	int i = 0;
	Json::Value sval = value["sprite"][i++];
	while (!sval.isNull()) {
		std::string filepath = sval["filepath"].asString().c_str();
		auto sym = SymbolMgr::Instance()->FetchSymbol(filepath);
		auto spr = SpriteFactory::Instance()->CreateRoot(sym);
		sym->RefreshThumbnail(filepath);
		sprs.push_back(spr);
		CopySprFromClipboard(spr, sval);
		InsertSpriteSJ::Instance()->Insert(spr);
		EditAddRecordSJ::Instance()->Add(std::make_shared<InsertSpriteAOP>(spr));
		sval = value["sprite"][i++];
	}

	for (int i = 0, n = sprs.size(); i < n; ++i) {
		m_selection->Add(sprs[i]);
	}
	SelectSpriteSetSJ::Instance()->Select(m_selection);

	wxTheClipboard->Close();
}

void SelectSpritesOP::GetOrderedSelection(std::vector<SprPtr>& sprs) const
{
	std::vector<SprPtr> selection;
	m_selection->Traverse(FetchAllRefVisitor<Sprite>(selection));
	if (selection.size() <= 1) {
		sprs = selection;
		return;
	}

	std::vector<SprPtr> all;
	m_sprs_impl->TraverseSprites(FetchAllRefVisitor<Sprite>(all));

	sprs.clear();
	for (int i = 0, n = all.size(); i < n; ++i) {
		for (int j = 0, m = selection.size(); j < m; ++j) {
			if (all[i] == selection[j]) {
				sprs.push_back(selection[j]);
				selection.erase(selection.begin() + j);
				break;
			}			
		}
	}
}

}