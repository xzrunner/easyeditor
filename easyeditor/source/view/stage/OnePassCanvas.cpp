#include "OnePassCanvas.h"

namespace ee
{

OnePassCanvas::OnePassCanvas(wxWindow* stage_wnd, EditPanelImpl* stage,
							 wxGLContext* glctx)
	: StageCanvas(stage_wnd, stage, glctx)
{
}

void OnePassCanvas::OnDrawWhole() const
{
 	glClearColor(m_bg_color.r / 255.0f, m_bg_color.g / 255.0f, m_bg_color.b / 255.0f, m_bg_color.a / 255.0f);
 	glClear(GL_COLOR_BUFFER_BIT);
 
 	OnDrawSprites();
}

}