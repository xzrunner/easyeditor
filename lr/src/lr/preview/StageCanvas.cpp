#include "StageCanvas.h"

namespace lr
{
namespace preview
{

StageCanvas::StageCanvas(d2d::EditPanel* stage, d2d::PlayControl& control,
						 const std::vector<const d2d::ISprite*>& sprites)
	: d2d::DynamicStageCanvas(stage)
	, m_control(control)
	, m_sprites(sprites)
{
}

void StageCanvas::InitGL()
{
	d2d::DynamicStageCanvas::InitGL();
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->getSymbol().ReloadTexture();
	}
}

void StageCanvas::OnDrawSprites() const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		const d2d::ISprite* sprite = m_sprites[i];
		// 		if (!sprite->visiable)
		// 			continue;
		d2d::SpriteRenderer::Instance()->Draw(sprite, d2d::Matrix(), sprite->multiCol, sprite->addCol);
	}
}

void StageCanvas::OnTimer()
{
	m_control.update();
}

}
}