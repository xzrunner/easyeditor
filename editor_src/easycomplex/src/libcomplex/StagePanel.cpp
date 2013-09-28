#include "StagePanel.h"

#include "StageCanvas.h"
#include "SelectSpritesOP.h"
#include "SymbolContainer.h"

namespace libcomplex
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame)
	: EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(parent, new SymbolContainer(m_symbol = new d2d::ComplexSymbol))
{
	m_editOP = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this);
	m_canvas = new StageCanvas(this);
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame,
					   d2d::ComplexSymbol* symbol)
	: EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(parent, new SymbolContainer(m_symbol = symbol))
{
	m_editOP = new d2d::ArrangeSpriteOP<SelectSpritesOP>(this, this);
	m_canvas = new StageCanvas(this);
}
} // libcomplex