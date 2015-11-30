#include "OnePassCanvas.h"

#include "render/RenderContext.h"
#include "render/ShaderMgrBase.h"

namespace d2d
{

OnePassCanvas::OnePassCanvas(wxWindow* stage_wnd, EditPanelImpl* stage)
	: IStageCanvas(stage_wnd, stage)
{
}

void OnePassCanvas::OnDrawWhole() const
{
 	glClearColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
 	glClear(GL_COLOR_BUFFER_BIT);
 
 	OnDrawSprites();
 
	RenderContext::Flush();
}

}