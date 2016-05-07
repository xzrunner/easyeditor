#include "PreviewCanvas.h"
#include "QueryWindowViewSizeSJ.h"

#include <ee/Sprite.h>
#include <ee/SpriteRenderer.h>
#include <ee/PlayControl.h>

namespace eui
{
namespace window
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage, ee::PlayControl& control,
							 const std::vector<const ee::Sprite*>& sprites, wxGLContext* glctx)
	: ee::CameraCanvas(stage_wnd, stage, glctx)
	, m_control(control)
	, m_sprites(sprites)
{
	int width, height;
	QueryWindowViewSizeSJ::Instance()->Query(width, height);
	float scale = std::min(width / QueryWindowViewSizeSJ::DEFAULT_VIEW_WIDTH, 
		height / QueryWindowViewSizeSJ::DEFAULT_VIEW_HEIGHT);
	m_scale_mt.Scale(scale, scale);
}

void PreviewCanvas::OnDrawSprites() const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		const ee::Sprite* spr = m_sprites[i];
		// 		if (!sprite->visiable)
		// 			continue;

		const ee::Vector& pos = spr->GetPosition();

		sm::mat4 inv_mt = spr->GetTransInvMatrix();
		sm::mat4 translate_mt;
		translate_mt.Translate(pos.x, pos.y);
		sm::mat4 mt = translate_mt * (m_scale_mt * inv_mt);

		ee::SpriteRenderer::Draw(spr, NULL, ee::RenderParams(mt, spr->rp->color));
	}
}

void PreviewCanvas::OnTimer()
{
	m_control.Update();
}

}
}