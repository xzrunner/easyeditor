#include "PreviewCanvas.h"

namespace libanim
{

PreviewCanvas::PreviewCanvas(d2d::EditPanel* stage,
							 const Symbol* symbol)
	: d2d::OrthoCanvas(stage)
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
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->ReloadTexture();
	}
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

	m_stage->SetCanvasDirty();
	if (m_setting.isCirculate) {
		m_control.reset();
	}
	else {
		m_control.decrease();
	}
}

} // anim
