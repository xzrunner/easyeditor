#include "PreviewCanvas.h"

#include <ee/SpriteRenderer.h>
#include <ee/Sprite.h>
#include <ee/SceneNode.h>

namespace ecomplex
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
							 const std::vector<const ee::Sprite*>& sprs,
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
		params.color = spr->Color();
		ee::SpriteRenderer::Draw(spr, params);
	}

	ee::SceneNodeMgr::Instance()->Draw();
}

}