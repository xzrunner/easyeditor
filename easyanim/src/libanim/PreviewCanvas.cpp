#include "PreviewCanvas.h"

#include <ee/SpriteRenderer.h>
#include <ee/panel_msg.h>

namespace eanim
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage,
							 const Symbol* symbol, wxGLContext* glctx)
	: ee::CameraCanvas(stage_wnd, stage, glctx)
	, m_symbol(symbol)
	, m_control(1.0f / symbol->getFPS())
{
}

PreviewCanvas::PlaySetting& PreviewCanvas::getPlaySetting()
{
	return m_setting;
}

void PreviewCanvas::OnDrawSprites() const
{
	ee::SpriteRenderer::Draw(m_symbol);
}

void PreviewCanvas::OnTimer()
{
// 	if (m_control.frame() < m_symbol->getMaxFrameIndex()) {
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
