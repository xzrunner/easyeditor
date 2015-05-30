#include "SelectSpritesOP.h"

#include "common/Rect.h"
#include "dataset/TextSprite.h"
#include "dataset/FontSprite.h"
#include "dataset/SymbolMgr.h"
#include "dataset/SpriteFactory.h"
#include "component/AbstractEditCMPT.h"
#include "view/PropertySettingPanel.h"
#include "view/SpritePropertySetting.h"
#include "view/MultiSpritesPropertySetting.h"
#include "view/TextPropertySetting.h"
#include "view/FontPropertySetting.h"
#include "view/MultiSpritesImpl.h"
#include "view/GLCanvas.h"
#include "render/DrawSelectedSpriteVisitor.h"
#include "render/PrimitiveDraw.h"
#include "render/style_config.h"

#include <wx/clipbrd.h>
#include <sstream>

namespace d2d
{

SelectSpritesOP::SelectSpritesOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
								 PropertySettingPanel* propertyPanel/* = NULL*/, AbstractEditCMPT* callback/* = NULL*/)
	: DrawRectangleOP(editPanel)
	, m_callback(callback)
	, m_spritesImpl(spritesImpl)
	, m_propertyPanel(propertyPanel)
	, m_bDraggable(true)
{
	m_selection = spritesImpl->getSpriteSelection();
	m_selection->Retain();

	m_firstPos.setInvalid();
}

SelectSpritesOP::~SelectSpritesOP()
{
	m_selection->Clear();
	m_selection->Release();
}

bool SelectSpritesOP::onKeyDown(int keyCode)
{
	if (DrawRectangleOP::onKeyDown(keyCode)) return true;

	if (wxGetKeyState(WXK_CONTROL) && wxGetKeyState(WXK_CONTROL_X))
	{
		PasteToSelection();
		m_spritesImpl->removeSpriteSelection();
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

bool SelectSpritesOP::onMouseLeftDown(int x, int y)
{
 	m_bDraggable = true;

	Vector pos = m_stage->transPosScreenToProject(x, y);
	ISprite* selected = selectByPos(pos);
	if (selected)
	{
		assert(selected->editable);
		if (wxGetKeyState(WXK_CONTROL))
		{
			if (m_selection->IsExist(selected))
				m_selection->Remove(selected);
			else
			{
				m_selection->Add(selected);
				if (m_propertyPanel)
				{
					if (m_selection->Size() == 1)
						m_propertyPanel->SetPropertySetting(createPropertySetting(selected));
					else if (m_selection->Size() > 1)
					{
						std::vector<ISprite*> sprites;
						m_selection->Traverse(FetchAllVisitor<ISprite>(sprites));
						m_propertyPanel->SetPropertySetting(createPropertySetting(sprites));
					}
					else
						m_propertyPanel->SetPropertySetting(createPropertySetting(NULL));
				}
			}
		}
		else
		{
			if (!m_selection->IsExist(selected) && !wxGetKeyState(WXK_SPACE))
			{
				m_selection->Clear();
				m_selection->Add(selected);
				if (m_propertyPanel)
					m_propertyPanel->SetPropertySetting(createPropertySetting(selected));
			}
		}
		m_firstPos.setInvalid();

		if (m_callback)
			m_callback->updateControlValue();
	}
	else
	{
		DrawRectangleOP::onMouseLeftDown(x, y);
		m_firstPos = pos;
		if (wxGetKeyState(WXK_CONTROL))
			m_bDraggable = false;
		else
			m_selection->Clear();
		m_stage->Refresh();
	}

	return false;
}

bool SelectSpritesOP::onMouseLeftUp(int x, int y)
{
	if (DrawRectangleOP::onMouseLeftUp(x, y)) return true;

	m_bDraggable = true;

	if (m_firstPos.isValid())
	{
		Vector end = m_stage->transPosScreenToProject(x, y);
		Rect rect(m_firstPos, end);
		std::vector<ISprite*> sprites;
		m_spritesImpl->querySpritesByRect(rect, m_firstPos.x < end.x, sprites);
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

		if (m_propertyPanel)
		{
			if (m_selection->Size() == 1)
				m_propertyPanel->SetPropertySetting(createPropertySetting(sprites[0]));
			else if (m_selection->Size() > 1)
				m_propertyPanel->SetPropertySetting(createPropertySetting(sprites));
			else
				m_propertyPanel->SetPropertySetting(createPropertySetting(NULL));
		}

		m_firstPos.setInvalid();

		if (m_callback)
			m_callback->updateControlValue();
	}

//	enableRightTap(m_selection->empty());

	return false;
}

bool SelectSpritesOP::onMouseRightDown(int x, int y)
{
	m_rightFirstScrPos.set(x, y);

	enableRightTap(m_selection->IsEmpty());

	if (DrawRectangleOP::onMouseRightDown(x, y)) return true;

	return false;
}

bool SelectSpritesOP::onMouseRightUp(int x, int y)
{
	// select
	if (m_rightFirstScrPos == Vector(x, y))
	{
		Vector pos = m_stage->transPosScreenToProject(x, y);
		d2d::ISprite* sprite = m_spritesImpl->querySpriteByPos(pos);
		if (sprite)
		{
			m_selection->Clear();
			m_selection->Add(sprite);
			enableRightTap(m_selection->IsEmpty());
			m_stage->Refresh();
		}
	}

	if (DrawRectangleOP::onMouseRightUp(x, y)) return true;

	return false;
}

bool SelectSpritesOP::onMouseDrag(int x, int y)
{
	if (DrawRectangleOP::onMouseDrag(x, y)) return true;

	return !m_bDraggable;
}

bool SelectSpritesOP::onDraw() const
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

// 	if (DrawRectangleOP::onDraw()) return true;

	return false;
}

bool SelectSpritesOP::clear()
{
	if (DrawRectangleOP::clear()) return true;

	m_selection->Clear();
	m_firstPos.setInvalid();

	return false;
}

IPropertySetting* SelectSpritesOP::createPropertySetting(ISprite* sprite) const
{
	if (TextSprite* text = dynamic_cast<TextSprite*>(sprite))
		return new TextPropertySetting(m_stage, text);
	else if (FontSprite* font = dynamic_cast<FontSprite*>(sprite))
		return new FontPropertySetting(m_stage, font);
	else if (sprite)
		return new SpritePropertySetting(m_stage, sprite);
	else 
		return NULL;
}

IPropertySetting* SelectSpritesOP::createPropertySetting(const std::vector<ISprite*>& sprites) const
{
	return new MultiSpritesPropertySetting(m_stage, sprites);
}

ISprite* SelectSpritesOP::selectByPos(const Vector& pos) const
{
	ISprite* selected = NULL;
	std::vector<ISprite*> sprites;
	m_spritesImpl->getSpriteSelection()->Traverse(FetchAllVisitor<ISprite>(sprites));
	for (int i = 0, n = sprites.size(); i < n; ++i)
	{
		if (sprites[i]->isContain(pos)) {
			selected = sprites[i];
			break;
		}
	}
	if (!selected) {
		ISprite* spr = m_spritesImpl->querySpriteByPos(pos);
		if (spr) {
			selected = spr;
		}
	}
	return selected;
}

void SelectSpritesOP::PasteSprToClipboard(const d2d::ISprite* spr, Json::Value& value) const
{
	value["filename"] = spr->getSymbol().getFilepath().ToStdString();
	spr->store(value);	
}

void SelectSpritesOP::CopySprFromClipboard(d2d::ISprite* spr, const Json::Value& value) const
{
	spr->load(value);
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
	if (wxTheClipboard->Open())
	{
		if (wxTheClipboard->IsSupported( wxDF_TEXT ))
		{
			wxTextDataObject data;
			wxTheClipboard->GetData( data );

			Json::Value value;
			Json::Reader reader;
			std::string test = data.GetText().ToStdString();
			reader.parse(data.GetText().ToStdString(), value);

			m_selection->Clear();

			int i = 0;
			Json::Value sval = value["sprite"][i++];
			while (!sval.isNull()) {
				std::string filepath = sval["filename"].asString();
				ISymbol* symbol = SymbolMgr::Instance()->fetchSymbol(filepath);
				// for snapshoot
				symbol->RefreshThumbnail(filepath);
				ISprite* sprite = SpriteFactory::Instance()->create(symbol);
				symbol->Release();
				CopySprFromClipboard(sprite, sval);
				m_spritesImpl->insertSprite(sprite);
				m_selection->Add(sprite);
				m_propertyPanel->SetPropertySetting(createPropertySetting(sprite));

				sval = value["sprite"][i++];
			}

			m_stage->getCanvas()->resetViewport();
		}
		wxTheClipboard->Close();
	}
}

} // d2d