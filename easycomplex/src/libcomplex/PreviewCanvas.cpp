#include "PreviewCanvas.h"

#include <ee/SpriteRenderer.h>
#include <ee/Sprite.h>

#include <sprite2/Particle3d.h>

namespace ecomplex
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
							 const std::vector<ee::Sprite*>& sprs,
							 wxGLContext* glctx)
	: ee::CameraCanvas(stage_wnd, stage, glctx)
	, m_sprs(sprs)
{
}

void PreviewCanvas::OnDrawSprites() const
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
	{
		const ee::Sprite* spr = m_sprs[i];
// 		if (!spr->visiable)
// 			continue;
		s2::RenderParams params;
		params.color = spr->GetColor();
		ee::SpriteRenderer::Instance()->Draw(spr, params);
	}

	s2::Particle3d::Instance()->Draw();
}

}