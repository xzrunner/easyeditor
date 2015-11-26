#include "PreviewCanvas.h"

namespace libanim
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, d2d::EditPanelImpl* stage,
							 const Symbol* symbol)
	: d2d::OrthoCanvas(stage_wnd, stage)
	, m_symbol(symbol)
	, m_control(1.0f / symbol->getFPS())
{
}

PreviewCanvas::PlaySetting& PreviewCanvas::getPlaySetting()
{
	return m_setting;
}

void PreviewCanvas::InitGL()
{
	d2d::OrthoCanvas::InitGL();

	d2d::ImageMgr::Instance()->Traverse(d2d::ReloadTextureVisitor<d2d::Image>());
}

void PreviewCanvas::OnDrawSprites() const
{
	d2d::SpriteRenderer::Instance()->Draw(m_symbol);
}

void PreviewCanvas::OnTimer()
{
// 	if (m_control.frame() < m_symbol->getMaxFrameIndex()) {
// 		return;
// 	}

	d2d::SetCanvasDirtySJ::Instance()->SetDirty();
	if (m_setting.isCirculate) {
		m_control.reset();
	}
	else {
		m_control.decrease();
	}
}

} // anim
