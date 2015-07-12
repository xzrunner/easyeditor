#include "PreviewCanvas.h"

#include <easyanim.h>

namespace ecomplex
{

PreviewCanvas::PreviewCanvas(d2d::EditPanel* stage, d2d::PlayControl& control,
							 const std::vector<const d2d::ISprite*>& sprites)
	: d2d::OrthoCanvas(stage)
	, m_control(control)
	, m_sprites(sprites)
{
}

void PreviewCanvas::InitGL()
{
	d2d::OrthoCanvas::InitGL();
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->GetSymbol().ReloadTexture();
	}
}

void PreviewCanvas::OnDrawSprites() const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		const d2d::ISprite* sprite = m_sprites[i];
// 		if (!sprite->visiable)
// 			continue;
		d2d::SpriteRenderer::Instance()->Draw(sprite, d2d::Matrix(), sprite->multiCol, sprite->addCol);
	}
}

void PreviewCanvas::OnTimer()
{
	m_control.update();
}

}