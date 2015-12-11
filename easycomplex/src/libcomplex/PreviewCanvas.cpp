#include "PreviewCanvas.h"

#include <easyanim.h>

namespace ecomplex
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, 
							 const std::vector<const d2d::ISprite*>& sprites,
							 wxGLContext* glctx)
	: d2d::OrthoCanvas(stage_wnd, stage, glctx)
	, m_sprites(sprites)
{
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

}