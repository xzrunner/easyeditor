#include "DialogStageCanvas.h"
#include "EditPanelImpl.h"

namespace ee
{

DialogStageCanvas::DialogStageCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, 
									 Symbol* sym, wxGLContext* glctx)
	: CameraCanvas(stage_wnd, stage, gum::CAM_ORTHO2D, glctx)
	, m_sym(sym)
{
}

void DialogStageCanvas::OnDrawSprites() const
{
	m_stage->DrawEditOP();
}

}