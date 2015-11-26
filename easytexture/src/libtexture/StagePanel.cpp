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
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
	, d2d::ShapesPanelImpl(new SymbolContainer(m_symbol = new Symbol))
{
	SetEditOP(new d2d::ZoomViewOP(this, GetStageImpl(), true));
	SetCanvas(new StageCanvas(this));
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   d2d::ISprite* edited, const d2d::MultiSpritesImpl* bg_sprites, 
					   d2d::LibraryPanel* library)
	: d2d::EditPanel(parent, frame)
	, d2d::SpritesPanelImpl(GetStageImpl(), library)
	, d2d::ShapesPanelImpl(new SymbolContainer((Symbol*)(&edited->GetSymbol())))
{
	SetEditOP(new libshape::EditPolylineOP<libshape::DrawPolygonOP, 
		d2d::SelectShapesOP>(this, GetStageImpl(), this, NULL, new d2d::OneFloatValueStatic(5), NULL));
	SetCanvas(new StageCanvas(this, edited, bg_sprites));

	m_symbol = (Symbol*)(&edited->GetSymbol());
	if (m_symbol) {
		m_symbol->Retain();
	}
}

StagePanel::~StagePanel()
{
	m_symbol->Release();
}

}