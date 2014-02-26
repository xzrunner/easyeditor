#include "StagePanel.h"
#include "StageCanvas.h"
#include "SelectSpritesOP.h"
#include "SymbolContainer.h"
#include "LibraryPanel.h"
#include "Symbol.h"

extern d2d::StageModule MODULE_STAGE;

namespace complex
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::PropertySettingPanel* property,
					   LibraryPanel* library)
	: EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(parent, new SymbolContainer(m_symbol = new Symbol))
	, m_library(library)
{
	m_editOP = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this, property);
	m_canvas = new StageCanvas(this);

	SetDropTarget(new DragSymbolTarget(this, library));

	MODULE_STAGE.impl = this;
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   Symbol* symbol,
					   d2d::PropertySettingPanel* property,
					   LibraryPanel* library)
	: EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(parent, new SymbolContainer(m_symbol = symbol))
	, m_library(library)
{
	m_editOP = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this, property);
	m_canvas = new StageCanvas(this);

	SetDropTarget(new DragSymbolTarget(this, library));
}

void StagePanel::clear()
{
	d2d::EditPanel::clear();

	Symbol* symbol = getSymbol();
	for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
		symbol->m_sprites[i]->release();
	symbol->m_sprites.clear();
}

void StagePanel::removeSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::removeSprite(sprite);
	m_viewlist->remove(sprite);
}

void StagePanel::insertSprite(d2d::ISprite* sprite)
{
	d2d::SpritesPanelImpl::insertSprite(sprite);
	m_viewlist->insert(sprite);
}

void StagePanel::resetSpriteOrder(d2d::ISprite* sprite, bool up)
{
	d2d::SpritesPanelImpl::resetSpriteOrder(sprite, up);
	m_viewlist->reorder(sprite, up);
}

//////////////////////////////////////////////////////////////////////////
// class StagePanel::DragSymbolTarget
//////////////////////////////////////////////////////////////////////////

StagePanel::DragSymbolTarget::
DragSymbolTarget(StagePanel* stage, d2d::LibraryPanel* library)
	: m_stage(stage)
	, m_library(library)
{
}

bool StagePanel::DragSymbolTarget::
OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	wxString sType = data.substr(0, data.find(","));
	wxString sIndex = data.substr(data.find(",") + 1);

	long index;
	sIndex.ToLong(&index);

	d2d::ISymbol* symbol = m_library->getSymbol(index);

// 	LibraryPanel* library = m_stage->m_library;
// 	d2d::ISymbol* symbol = NULL;
// 	if (sType == "symbol")
// 		symbol = static_cast<d2d::ISymbol*>(library->getImagePage()->getSymbol(index));
// 	else if (sType == "complex")
// 		symbol = static_cast<d2d::ISymbol*>(library->getComplexPage()->getSymbol(index));
// 	else if (sType == "anim")
// 		symbol = static_cast<d2d::ISymbol*>(library->getAnimPage()->getSymbol(index));
// 	else if (sType == "9patch")
// 		symbol = static_cast<d2d::ISymbol*>(library->get9PatchPage()->getSymbol(index));
// 	else if (sType == "fontblank")
// 		symbol = static_cast<d2d::ISymbol*>(library->getFontPage()->getSymbol(index));
// 	else if (sType == "scripts")
// 		symbol = static_cast<d2d::ISymbol*>(library->getScriptsPage()->getSymbol(index));

	if (symbol)
	{
		if (d2d::ScriptsSymbol* scripts = dynamic_cast<d2d::ScriptsSymbol*>(symbol)) 
		{
			d2d::scripts_do_string(scripts->getContent().c_str());
//			d2d::scripts_do_file(symbol->getFilepath().c_str());
		}
		else
		{
			d2d::Vector pos = m_stage->transPosScreenToProject(x, y);
			d2d::ISprite* sprite = d2d::SpriteFactory::Instance()->create(symbol);
			sprite->translate(pos);
			m_stage->insertSprite(sprite);
		}
	}

	return true;
}

} // complex