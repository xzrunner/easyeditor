#include "OnePassCanvas.h"
#include "ShaderContext.h"
#include "ShaderMgrBase.h"

namespace ee
{

OnePassCanvas::OnePassCanvas(wxWindow* stage_wnd, EditPanelImpl* stage,
							 wxGLContext* glctx)
	: StageCanvas(stage_wnd, stage, glctx)
{
}

void OnePassCanvas::OnDrawWhole() const
{
 	glClearColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
 	glClear(GL_COLOR_BUFFER_BIT);
 
 	OnDrawSprites();
 
	ShaderContext::Flush();
}

}