#include "SelectSpritesOP.h"

#include "common/Rect.h"
#include "common/sprite_visitors.h"
#include "dataset/TextSprite.h"
#include "dataset/FontSprite.h"
#include "dataset/SymbolMgr.h"
#include "dataset/SpriteFactory.h"
#include "component/AbstractEditCMPT.h"
#include "view/MultiSpritesImpl.h"
#include "view/IStageCanvas.h"
#include "view/SpriteSelection.h"
#include "view/EditPanelImpl.h"
#include "render/DrawSelectedSpriteVisitor.h"
#include "render/PrimitiveDraw.h"
#include "message/SelectSpriteSJ.h"
#include "message/SelectSpriteSetSJ.h"
#include "message/InsertSpriteSJ.h"

#include <wx/clipbrd.h>
#include <sstream>

namespace d2d
{

SelectSpritesOP::SelectSpritesOP(wxWindow* wnd, d2d::EditPanelImpl* stage, 
								 MultiSpritesImpl* spritesImpl, 
								 AbstractEditCMPT* callback)
	: DrawSelectRectOP(wnd, stage)
	, m_callback(callback)
	, m_spritesImpl(spritesImpl)
	, m_bDraggable(true)
{
	m_selection = spritesImpl->GetSpriteSelection();
	m_selection->Retain();

	m_first_pos.setInvalid();
}

SelectSpritesOP::~SelectSpritesOP()
{
	m_selection->Clear();
	m_selection->Release();
}

bool SelectSpritesOP::OnKeyDown(int keyCode)
{
	if (DrawSelectRectOP::OnKeyDown(keyCode)) return true;

	if (m_stage->GetKeyState(WXK_CONTROL) && m_stage->GetKeyState(WXK_CONTROL_X))
	{
		PasteToSelection();
		m_spritesImpl->ClearSpriteSelection();
		return true;
	}
	else if (m_stage->GetKeyState(WXK_CONTROL) && (keyCode == 'c' || keyCode == 'C'))
	{
		PasteToSelection();
		return true;
	}
	else if (m_stage->GetKeyState(WXK_CONTROL) && (keyCode == 'v' || keyCode == 'V'))
	{
		CopyFromSelection();
		return true;
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftDown(int x, int y)
{
 	m_bDraggable = true;

	Vector pos = m_stage->TransPosScrToProj(x, y);
	ISprite* selected = SelectByPos(pos);
	if (selected)
	{
		assert(selected->editable);
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
				m_selection->Add(selected);

				bool add = m_stage->GetKeyState(WXK_CONTROL);
				SelectSpriteSJ::Instance()->Select(selected, !add);
			}
		}
		m_first_pos.setInvalid();

		if (m_callback)
			m_callback->updateControlValue();
	}
	else
	{
		DrawSelectRectOP::OnMouseLeftDown(x, y);

		if (m_stage->GetKeyState(WXK_SPACE)) {
			m_first_pos.setInvalid();
		} else {
			m_first_pos = pos;
		}

		if (m_stage->GetKeyState(WXK_CONTROL)) {
			m_bDraggable = false;
		} else {
			m_selection->Clear();
		}
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftUp(int x, int y)
{
	if (DrawSelectRectOP::OnMouseLeftUp(x, y)) return true;

	m_bDraggable = true;

	if (!m_first_pos.isValid()) {
		return false;
	}

	Vector end = m_stage->TransPosScrToProj(x, y);
	Rect rect(m_first_pos, end);
	std::vector<ISprite*> sprites;
	m_spritesImpl->QuerySpritesByRect(rect, m_first_pos.x < end.x, sprites);
	if (m_stage->GetKeyState(WXK_CONTROL))
	{
		for (size_t i = 0, n = sprites.size(); i < n; ++i) 
		{
			d2d::ISprite* sprite = sprites[i];
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

	m_first_pos.setInvalid();

	if (m_callback) {
		m_callback->updateControlValue();
	}

	m_stage->SetCanvasDirty();

	return false;
}

bool SelectSpritesOP::OnMouseRightDown(int x, int y)
{
	m_rightFirstScrPos.set(x, y);

	SetRightPan(m_selection->IsEmpty());
	if (DrawSelectRectOP::OnMouseRightDown(x, y)) return true;

	return false;
}

bool SelectSpritesOP::OnMouseRightUp(int x, int y)
{
	// select
	if (m_rightFirstScrPos == Vector(x, y))
	{
		Vector pos = m_stage->TransPosScrToProj(x, y);
		PointMultiQueryVisitor visitor(pos);
		m_spritesImpl->TraverseSprites(visitor, DT_EDITABLE);
		const std::vector<ISprite*>& sprites = visitor.GetResult();
		SetRightPan(sprites.empty());
	}

	if (DrawSelectRectOP::OnMouseRightUp(x, y)) return true;

	return false;
}

bool SelectSpritesOP::OnMouseDrag(int x, int y)
{
	if (DrawSelectRectOP::OnMouseDrag(x, y)) return true;

	return !m_bDraggable;
}

bool SelectSpritesOP::OnDraw() const
{
	if (DrawSelectRectOP::OnDraw()) {
		return true;
	}

	m_selection->Traverse(DrawSelectedSpriteVisitor(Colorf(1, 0, 0)));

	return false;
}

bool SelectSpritesOP::Clear()
{
	if (DrawSelectRectOP::Clear()) return true;

	m_selection->Clear();
	m_first_pos.setInvalid();

	return false;
}

ISprite* SelectSpritesOP::SelectByPos(const Vector& pos) const
{
	if (m_stage->GetKeyState(WXK_SPACE)) {
		return NULL;
	}

	ISprite* selected = NULL;
	std::vector<ISprite*> sprites;
	m_spritesImpl->GetSpriteSelection()->Traverse(FetchAllVisitor<ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		ISprite* spr = sprites[i];
		if (spr->editable && spr->IsContain(pos)) {
			selected = spr;
			break;
		}
	}
	if (!selected) {
		ISprite* spr = m_spritesImpl->QuerySpriteByPos(pos);
		if (spr) {
			selected = spr;
		}
	}
	return selected;
}

void SelectSpritesOP::PasteSprToClipboard(const d2d::ISprite* spr, Json::Value& value) const
{
	value["filename"] = spr->GetSymbol().GetFilepath();
	spr->Store(value);	
}

void SelectSpritesOP::CopySprFromClipboard(d2d::ISprite* spr, const Json::Value& value) const
{
	spr->Load(value);
}

void SelectSpritesOP::PasteToSelection() const
{
	std::vector<ISprite*> sprites;
	m_selection->Traverse(FetchAllVisitor<ISprite>(sprites));
	Json::Value value;
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		Json::Value& sval = value["sprite"][i];
		d2d::ISprite* s = sprites[i];
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
		return;
	}
 
	m_selection->Clear();

	ISprite* last_spr = NULL;

	std::vector<ISprite*> sprites;

	int i = 0;
	Json::Value sval = value["sprite"][i++];
	while (!sval.isNull()) {
		std::string filepath = sval["filename"].asString();
		ISymbol* symbol = SymbolMgr::Instance()->FetchSymbol(filepath);
		// for snapshoot
		symbol->RefreshThumbnail(filepath);
		ISprite* sprite = SpriteFactory::Instance()->create(symbol);
		sprites.push_back(sprite);
		symbol->Release();
		CopySprFromClipboard(sprite, sval);

		InsertSpriteSJ::Instance()->Insert(spr);

		last_spr = sprite;

		sval = value["sprite"][i++];
	}

	for (int i = 0, n = sprites.size(); i < n; ++i) {
		m_selection->Add(sprites[i]);
	}

	bool add = m_stage->GetKeyState(WXK_CONTROL);
	SelectSpriteSJ::Instance()->Select(last_spr, !add);

	m_stage->GetCanvas()->ResetViewport();

	wxTheClipboard->Close();
}

} // d2d