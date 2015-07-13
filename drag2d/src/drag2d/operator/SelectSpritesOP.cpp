#include "SelectSpritesOP.h"

#include "common/Rect.h"
#include "dataset/TextSprite.h"
#include "dataset/FontSprite.h"
#include "dataset/SymbolMgr.h"
#include "dataset/SpriteFactory.h"
#include "component/AbstractEditCMPT.h"
#include "view/MultiSpritesImpl.h"
#include "view/IStageCanvas.h"
#include "view/ViewPanelMgr.h"
#include "view/SpriteSelection.h"
#include "render/DrawSelectedSpriteVisitor.h"
#include "render/PrimitiveDraw.h"
#include "render/style_config.h"

#include <wx/clipbrd.h>
#include <sstream>

namespace d2d
{

SelectSpritesOP::SelectSpritesOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
								 ViewPanelMgr* view_panel_mgr, AbstractEditCMPT* callback)
	: DrawRectangleOP(editPanel)
	, m_callback(callback)
	, m_spritesImpl(spritesImpl)
	, m_bDraggable(true)
	, m_view_panel_mgr(view_panel_mgr)
{
	m_selection = spritesImpl->GetSpriteSelection();
	m_selection->Retain();

	m_firstPos.setInvalid();
}

SelectSpritesOP::~SelectSpritesOP()
{
	m_selection->Clear();
	m_selection->Release();
}

bool SelectSpritesOP::OnKeyDown(int keyCode)
{
	if (DrawRectangleOP::OnKeyDown(keyCode)) return true;

	if (wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_CONTROL_X))
	{
		PasteToSelection();
		m_spritesImpl->ClearSpriteSelection();
		return true;
	}
	else if (wxGetKeyState(WXK_CONTROL) && (keyCode == 'c' || keyCode == 'C'))
	{
		PasteToSelection();
		return true;
	}
	else if (wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_CONTROL_V))
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
		if (wxGetKeyState(WXK_CONTROL))
		{
			if (m_selection->IsExist(selected)) {
				m_selection->Remove(selected);
			} else {
				m_selection->Add(selected);
			}
			if (m_view_panel_mgr) {
				m_view_panel_mgr->SelectMultiSprites(m_selection, m_spritesImpl);
			}
		}
		else
		{
			if (!m_selection->IsExist(selected) && !wxGetKeyState(WXK_SPACE))
			{
				m_selection->Clear();
				m_selection->Add(selected);
				if (m_view_panel_mgr) {
					bool add = wxGetKeyState(WXK_CONTROL);
					m_view_panel_mgr->SelectSprite(selected, !add, m_spritesImpl);
				}
			}
		}
		m_firstPos.setInvalid();

		if (m_callback)
			m_callback->updateControlValue();
	}
	else
	{
		DrawRectangleOP::OnMouseLeftDown(x, y);
		m_firstPos = pos;
		if (wxGetKeyState(WXK_CONTROL))
			m_bDraggable = false;
		else
			m_selection->Clear();
	}

	return false;
}

bool SelectSpritesOP::OnMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::OnMouseLeftUp(x, y)) return true;

	m_bDraggable = true;

	if (!m_firstPos.isValid()) {
		return false;
	}

	Vector end = m_stage->TransPosScrToProj(x, y);
	Rect rect(m_firstPos, end);
	std::vector<ISprite*> sprites;
	m_spritesImpl->QuerySpritesByRect(rect, m_firstPos.x < end.x, sprites);
	if (wxGetKeyState(WXK_CONTROL))
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

	if (m_view_panel_mgr) {
		m_view_panel_mgr->SelectMultiSprites(m_selection, m_spritesImpl);
	}

	m_firstPos.setInvalid();

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

	if (DrawRectangleOP::OnMouseRightDown(x, y)) return true;

	return false;
}

bool SelectSpritesOP::OnMouseRightUp(int x, int y)
{
	// select
	if (m_rightFirstScrPos == Vector(x, y))
	{
		Vector pos = m_stage->TransPosScrToProj(x, y);
		d2d::ISprite* sprite = m_spritesImpl->QuerySpriteByPos(pos);
		if (sprite)
		{
			m_selection->Clear();
			m_selection->Add(sprite);
			SetRightPan(m_selection->IsEmpty());

		}
	}

	if (DrawRectangleOP::OnMouseRightUp(x, y)) return true;

	return false;
}

bool SelectSpritesOP::OnMouseDrag(int x, int y)
{
	if (DrawRectangleOP::OnMouseDrag(x, y)) return true;

	return !m_bDraggable;
}

bool SelectSpritesOP::OnDraw() const
{
	m_selection->Traverse(DrawSelectedSpriteVisitor(Colorf(1, 0, 0)));

	if (m_firstPos.isValid() && m_currPos.isValid())
	{
		if (m_currPos.x > m_firstPos.x)
		{
			PrimitiveDraw::rect(m_firstPos, m_currPos, SELECT_ALL);
			PrimitiveDraw::rect(m_firstPos, m_currPos, SELECT_BOUND);
		}
		else
		{
			PrimitiveDraw::rect(m_firstPos, m_currPos, SELECT_PART);
			PrimitiveDraw::rect(m_firstPos, m_currPos, SELECT_BOUND);
		}
	}

// 	if (DrawRectangleOP::OnDraw()) return true;

	return false;
}

bool SelectSpritesOP::Clear()
{
	if (DrawRectangleOP::Clear()) return true;

	m_selection->Clear();
	m_firstPos.setInvalid();

	return false;
}

ISprite* SelectSpritesOP::SelectByPos(const Vector& pos) const
{
	ISprite* selected = NULL;
	std::vector<ISprite*> sprites;
	m_spritesImpl->GetSpriteSelection()->Traverse(FetchAllVisitor<ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		if (sprites[i]->IsContain(pos)) {
			selected = sprites[i];
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

	m_selection->Clear();

	ISprite* last_spr = NULL;

	int i = 0;
	Json::Value sval = value["sprite"][i++];
	while (!sval.isNull()) {
		std::string filepath = sval["filename"].asString();
		ISymbol* symbol = SymbolMgr::Instance()->FetchSymbol(filepath);
		// for snapshoot
		symbol->RefreshThumbnail(filepath);
		ISprite* sprite = SpriteFactory::Instance()->create(symbol);
		symbol->Release();
		CopySprFromClipboard(sprite, sval);
		m_spritesImpl->InsertSprite(sprite);
		m_selection->Add(sprite);
		last_spr = sprite;

		sval = value["sprite"][i++];
	}

	bool add = wxGetKeyState(WXK_CONTROL);
	m_view_panel_mgr->SelectSprite(last_spr, !add, m_spritesImpl);

	m_stage->GetCanvas()->ResetViewport();

	wxTheClipboard->Close();
}

} // d2d