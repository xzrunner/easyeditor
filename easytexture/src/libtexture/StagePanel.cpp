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
	, ee::ShapesPanelImpl(std::make_shared<SymbolContainer>(m_sym = std::make_shared<Symbol>()))
{
	SetEditOP(std::make_shared<ee::ZoomViewOP>(this, GetStageImpl(), true));
	SetCanvas(std::make_shared<StageCanvas>(this));
}

StagePanel::StagePanel(wxWindow* parent, wxTopLevelWindow* frame, 
					   wxGLContext* glctx, const ee::SprPtr& edited, 
					   const ee::MultiSpritesImpl* bg_sprites, 
					   ee::LibraryPanel* library)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), library)
	, ee::ShapesPanelImpl(std::make_shared<SymbolContainer>(std::dynamic_pointer_cast<Symbol>(edited->GetSymbol())))
{
	SetEditOP(std::make_shared<eshape::EditPolylineOP<eshape::DrawPolygonOP, ee::SelectShapesOP>>(
		this, GetStageImpl(), this, nullptr, new ee::OneFloatValueStatic(5), nullptr));
	SetCanvas(std::make_shared<StageCanvas>(this, glctx, edited, bg_sprites));

	m_sym = std::dynamic_pointer_cast<Symbol>(edited->GetSymbol());
}

void StagePanel::OnKeyHook(int key_code)
{
	if (GetKeyState(WXK_CONTROL) && key_code == 'S') {
		FileSaver::Store(m_sym->GetFilepath().c_str(), m_sym);
	}
}

}