#include "PreviewCanvas.h"
#include "QueryWindowViewSizeSJ.h"

namespace eui
{
namespace window
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, d2d::PlayControl& control,
							 const std::vector<const d2d::ISprite*>& sprites)
	: d2d::OrthoCanvas(stage_wnd, stage)
	, m_control(control)
	, m_sprites(sprites)
{
	int width, height;
	QueryWindowViewSizeSJ::Instance()->Query(width, height);
	float scale = std::min(width / QueryWindowViewSizeSJ::DEFAULT_VIEW_WIDTH, 
		height / QueryWindowViewSizeSJ::DEFAULT_VIEW_HEIGHT);
	m_scale_mt.scale(scale, scale);
}

void PreviewCanvas::OnDrawSprites() const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		const d2d::ISprite* sprite = m_sprites[i];
		// 		if (!sprite->visiable)
		// 			continue;

		const d2d::Vector& pos = sprite->GetPosition();

		d2d::Matrix inv_mt = sprite->GetTransInvMatrix();
		d2d::Matrix translate_mt;
		translate_mt.translate(pos.x, pos.y);
		d2d::Matrix mt = translate_mt * (m_scale_mt * inv_mt);

		d2d::SpriteRenderer::Instance()->Draw(sprite, mt, sprite->multiCol, sprite->addCol);
	}
}

void PreviewCanvas::OnTimer()
{
	m_control.update();
}

}
}