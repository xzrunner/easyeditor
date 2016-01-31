#include "PreviewCanvas.h"

#include <easyanim.h>

namespace ecomplex
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
							 const std::vector<const ee::Sprite*>& sprites,
							 wxGLContext* glctx)
	: ee::OrthoCanvas(stage_wnd, stage, glctx)
	, m_sprites(sprites)
{
}

void PreviewCanvas::OnDrawSprites() const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		const ee::Sprite* sprite = m_sprites[i];
// 		if (!sprite->visiable)
// 			continue;
		ee::SpriteRenderer::Instance()->Draw(sprite, NULL, ee::Matrix(), sprite->color);
	}
}

}