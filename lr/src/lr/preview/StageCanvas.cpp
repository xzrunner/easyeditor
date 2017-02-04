#include "StageCanvas.h"

#include <ee/SpriteRenderer.h>
#include <ee/Sprite.h>
#include <ee/PlayControl.h>

namespace lr
{
namespace preview
{

StageCanvas::StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, ee::PlayControl& control,
						 const std::vector<ee::Sprite*>& sprs, wxGLContext* glctx)
	: ee::CameraCanvas(stage_wnd, stage, s2::CAM_ORTHO2D, glctx)
	, m_control(control)
	, m_sprs(sprs)
{
}

void StageCanvas::OnDrawSprites() const
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
}

void StageCanvas::OnTimer()
{
	m_control.Update();
}

}
}