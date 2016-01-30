#include "PreviewCanvas.h"

namespace emap
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, 
							 d2d::PlayControl& control, const std::vector<const d2d::Sprite*>& sprites)
	: d2d::OrthoCanvas(stage_wnd, stage)
	, m_control(control)
	, m_sprites(sprites)
{
}

void PreviewCanvas::OnDrawSprites() const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		const d2d::Sprite* sprite = m_sprites[i];
		// 		if (!sprite->visiable)
		// 			continue;
		d2d::SpriteRenderer::Instance()->Draw(sprite, NULL, d2d::Matrix(), sprite->color);
	}
}

void PreviewCanvas::OnTimer()
{
	m_control.Update();
}

}