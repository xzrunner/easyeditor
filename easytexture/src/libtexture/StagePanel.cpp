#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "SymbolContainer.h"

namespace etexture
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, d2d::ShapesPanelImpl(this, new SymbolContainer(m_symbol = new Symbol))
{
	m_editOP = new d2d::ZoomViewOP(this, true);
	m_canvas = new StageCanvas(this);
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   Symbol* symbol, d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, d2d::ShapesPanelImpl(this, new SymbolContainer(symbol))
{	
	if (symbol) {
		m_symbol = symbol;
		symbol->Retain();
	}

	m_editOP = new d2d::ZoomViewOP(this, true);
	m_canvas = new StageCanvas(this);
}

StagePanel::~StagePanel()
{
	m_symbol->Release();
}

void StagePanel::clear()
{
}

}