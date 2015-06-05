#include "PreviewCanvas.h"

namespace libanim
{

PreviewCanvas::PreviewCanvas(d2d::EditPanel* stage,
							 const Symbol* symbol)
	: d2d::DynamicStageCanvas(stage)
	, m_symbol(symbol)
	, m_control(1.0f / symbol->getFPS())
{
}

PreviewCanvas::PlaySetting& PreviewCanvas::getPlaySetting()
{
	return m_setting;
}

void PreviewCanvas::initGL()
{
	d2d::DynamicStageCanvas::initGL();
	d2d::SymbolMgr::Instance()->traverse(d2d::ReloadTextureVisitor<d2d::ISymbol>());
}

void PreviewCanvas::onDraw()
{
	d2d::SpriteRenderer::Instance()->Draw(m_symbol);
}

void PreviewCanvas::OnTimer()
{
	if (m_control.frame() < m_symbol->getMaxFrameIndex()) {
		return;
	}

	if (m_setting.isCirculate) {
		m_control.reset();
	}
	else {
		m_control.decrease();
	}
}

} // anim
