#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "SymbolContainer.h"

#include <easyshape.h>

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
					   d2d::ISprite* edited, const std::vector<d2d::ISprite*>& bg_sprites, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(this, library)
	, d2d::ShapesPanelImpl(this, new SymbolContainer((Symbol*)(&edited->getSymbol())))
{
	m_editOP = new libshape::EditPolylineOP<libshape::DrawPolygonOP, 
		d2d::SelectShapesOP>(this, this, NULL, new d2d::OneFloatValueStatic(5), NULL);
	m_canvas = new StageCanvas(this, edited, bg_sprites);

	m_symbol = (Symbol*)(&edited->getSymbol());
	if (m_symbol) {
		m_symbol->Retain();
	}
}

StagePanel::~StagePanel()
{
	m_symbol->Release();
}

void StagePanel::clear()
{
}

}