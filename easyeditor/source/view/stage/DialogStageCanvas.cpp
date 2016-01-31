#include "DialogStageCanvas.h"
#include "EditPanelImpl.h"

namespace ee
{

DialogStageCanvas::DialogStageCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, 
									 Symbol* symbol, wxGLContext* glctx)
	: OrthoCanvas(stage_wnd, stage, glctx)
	, m_symbol(symbol)
{
}

void DialogStageCanvas::OnDrawSprites() const
{
	m_stage->DrawEditOP();
}

}