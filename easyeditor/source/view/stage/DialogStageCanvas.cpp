#include "DialogStageCanvas.h"
#include "EditPanelImpl.h"

#include <sprite2/CameraType.h>

namespace ee
{

DialogStageCanvas::DialogStageCanvas(wxWindow* stage_wnd, EditPanelImpl* stage, 
									 Symbol* sym, wxGLContext* glctx)
	: CameraCanvas(stage_wnd, stage, s2::CAM_ORTHO2D, glctx)
	, m_sym(sym)
{
}

void DialogStageCanvas::OnDrawSprites() const
{
	m_stage->DrawEditOP();
}

}