#include "StagePanel.h"
#include "StageCanvas.h"
#include "Symbol.h"
#include "SymbolContainer.h"
#include "FileSaver.h"

#include <easyshape.h>

#include <ee/SelectShapesOP.h>

namespace etexture
{

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
	, ee::ShapesPanelImpl(new SymbolContainer(m_sym = new Symbol))
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
	, ee::ShapesPanelImpl(new SymbolContainer(dynamic_cast<Symbol*>(edited->GetSymbol())))
{
	ee::EditOP* op = new eshape::EditPolylineOP<eshape::DrawPolygonOP, ee::SelectShapesOP>(this, 
		GetStageImpl(), this, NULL, new ee::OneFloatValueStatic(5), NULL);
	SetEditOP(op);
	op->RemoveReference();

	StageCanvas* canvas = new StageCanvas(this, glctx, edited, bg_sprites);
	SetCanvas(canvas);

	m_sym = dynamic_cast<Symbol*>(edited->GetSymbol());
	if (m_sym) {
		m_sym->AddReference();
	}
}

StagePanel::~StagePanel()
{
	m_sym->RemoveReference();
}

void StagePanel::OnKeyHook(int key_code)
{
	if (GetKeyState(WXK_CONTROL) && key_code == 'S') {
		FileSaver::Store(m_sym->GetFilepath().c_str(), m_sym);
	}
}

}