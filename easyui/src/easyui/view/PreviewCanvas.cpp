#include "PreviewCanvas.h"

#include "frame/SettingCfg.h"

namespace eui
{

const float PreviewCanvas::VIEW_WIDTH = 1024;
const float PreviewCanvas::VIEW_HEIGHT = 768;

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, d2d::PlayControl& control,
							 const std::vector<const d2d::ISprite*>& sprites)
	: d2d::OrthoCanvas(stage_wnd, stage)
	, m_control(control)
	, m_sprites(sprites)
{
	SettingCfg* cfg = SettingCfg::Instance();
	float scale = std::min(cfg->m_view_width / VIEW_WIDTH, cfg->m_view_height / VIEW_HEIGHT);
	m_scale_mt.scale(scale, scale);
}

void PreviewCanvas::InitGL()
{
	d2d::OrthoCanvas::InitGL();

	d2d::ImageMgr::Instance()->Traverse(d2d::ReloadTextureVisitor<d2d::Image>());
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->ReloadTexture();
	}
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