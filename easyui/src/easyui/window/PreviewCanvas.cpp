#include "PreviewCanvas.h"

#include <easyui.h>

#include <ee/Sprite.h>
#include <ee/SpriteRenderer.h>
#include <ee/PlayControl.h>

#include <sprite2/CameraType.h>

namespace eui
{
namespace window
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, ee::PlayControl& control,
							 const std::vector<ee::SprPtr>& sprs, wxGLContext* glctx)
	: ee::CameraCanvas(stage_wnd, stage, s2::CAM_ORTHO2D, glctx)
	, m_control(control)
	, m_sprs(sprs)
{
	int width, height;
	QueryWindowViewSizeSJ::Instance()->Query(width, height);
	float scale = std::min(width / QueryWindowViewSizeSJ::DEFAULT_VIEW_WIDTH, 
		height / QueryWindowViewSizeSJ::DEFAULT_VIEW_HEIGHT);
	m_scale_mt.Scale(scale, scale);
}

void PreviewCanvas::OnDrawSprites() const
{
	for (size_t i = 0, n = m_sprs.size(); i < n; ++i)
	{
		const ee::SprConstPtr& spr = m_sprs[i];
		// 		if (!spr->visiable)
		// 			continue;

		const sm::vec2& pos = spr->GetPosition();

		S2_MAT inv_mt = spr->GetLocalMat().Inverted();
		S2_MAT translate_mt;
		translate_mt.Translate(pos.x, pos.y);
		S2_MAT mt = translate_mt * (m_scale_mt * inv_mt);

		s2::RenderParams params;
		params.mt = mt;
		params.color = spr->GetColor();
		ee::SpriteRenderer::Instance()->Draw(spr.get(), params);
	}
}

void PreviewCanvas::OnTimer()
{
	m_control.Update();
}

}
}