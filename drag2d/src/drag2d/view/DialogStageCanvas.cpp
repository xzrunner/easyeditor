#include "DialogStageCanvas.h"

#include "common/config.h"
#include "dataset/ISymbol.h"
#include "view/EditPanelImpl.h"
#include "render/DrawCallBatching.h"

namespace d2d
{

DialogStageCanvas::DialogStageCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, 
									 ISymbol* symbol, wxGLContext* glctx)
	: OrthoCanvas(stage_wnd, stage, glctx)
	, m_symbol(symbol)
{
}

void DialogStageCanvas::OnDrawSprites() const
{
	m_stage->DrawEditOP();
}

} // d2d