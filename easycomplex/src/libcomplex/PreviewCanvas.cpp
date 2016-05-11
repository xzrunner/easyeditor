#include "PreviewCanvas.h"

#include <ee/SpriteRenderer.h>
#include <ee/Sprite.h>

namespace ecomplex
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
							 const std::vector<const ee::Sprite*>& sprites,
							 wxGLContext* glctx)
	: ee::CameraCanvas(stage_wnd, stage, glctx)
	, m_sprites(sprites)
{
}

void PreviewCanvas::OnDrawSprites() const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		const ee::Sprite* spr = m_sprites[i];
// 		if (!sprite->visiable)
// 			continue;
		ee::SpriteRenderer::Draw(spr, NULL, 
			ee::RenderParams(sm::mat4(), spr->GetColor()));
	}
}

}