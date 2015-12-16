#include "StageCanvas.h"

namespace lr
{
namespace preview
{

StageCanvas::StageCanvas(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, d2d::PlayControl& control,
						 const std::vector<const d2d::ISprite*>& sprites)
	: d2d::OrthoCanvas(stage_wnd, stage)
	, m_control(control)
	, m_sprites(sprites)
{
}

void StageCanvas::OnDrawSprites() const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		const d2d::ISprite* sprite = m_sprites[i];
		// 		if (!sprite->visiable)
		// 			continue;
		d2d::SpriteRenderer::Instance()->Draw(sprite, NULL, d2d::Matrix(), sprite->color);
	}
}

void StageCanvas::OnTimer()
{
	m_control.update();
}

}
}