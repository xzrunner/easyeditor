#include "PreviewCanvas.h"

#include <ee/SpriteRenderer.h>
#include <ee/panel_msg.h>

namespace libanim
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage,
							 const Symbol* sym, wxGLContext* glctx)
	: ee::CameraCanvas(stage_wnd, stage, gum::CAM_ORTHO2D, glctx)
	, m_sym(sym)
	, m_control(1.0f / sym->GetFPS())
{
}

PreviewCanvas::PlaySetting& PreviewCanvas::getPlaySetting()
{
	return m_setting;
}

void PreviewCanvas::OnDrawSprites() const
{
	ee::SpriteRenderer::Instance()->Draw(m_sym);
}

void PreviewCanvas::OnTimer()
{
// 	if (m_control.frame() < m_sym->getMaxFrameIndex()) {
// 		return;
// 	}

	ee::SetCanvasDirtySJ::Instance()->SetDirty();
	if (m_setting.isCirculate) {
		m_control.Reset();
	}
	else {
		m_control.Decrease();
	}
}

} // anim
