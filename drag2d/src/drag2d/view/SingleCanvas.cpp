#include "SingleCanvas.h"

#include "render/RenderContext.h"
#include "render/ShaderMgrBase.h"

namespace d2d
{

SingleCanvas::SingleCanvas(EditPanel* stage)
	: IStageCanvas(stage)
{
}

void SingleCanvas::OnDrawWhole() const
{
 	glClearColor(m_bg_color.r, m_bg_color.g, m_bg_color.b, m_bg_color.a);
 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
 	OnDrawSprites();
 
 	if (RenderContext::SHADER_MGR) {
 		RenderContext::SHADER_MGR->Flush();
 	}
}

}