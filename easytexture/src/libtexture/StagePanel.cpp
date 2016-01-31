#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "SymbolContainer.h"

#include <easyshape.h>

namespace etexture
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
	, ee::ShapesPanelImpl(new SymbolContainer(m_symbol = new Symbol))
{
	SetEditOP(new ee::ZoomViewOP(this, GetStageImpl(), true));
	SetCanvas(new StageCanvas(this));
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   wxGLContext* glctx, ee::Sprite* edited, 
					   const ee::MultiSpritesImpl* bg_sprites, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
	, ee::ShapesPanelImpl(new SymbolContainer((Symbol*)(&edited->GetSymbol())))
{
	SetEditOP(new libshape::EditPolylineOP<libshape::DrawPolygonOP, 
		ee::SelectShapesOP>(this, GetStageImpl(), this, NULL, new ee::OneFloatValueStatic(5), NULL));
	SetCanvas(new StageCanvas(this, glctx, edited, bg_sprites));

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