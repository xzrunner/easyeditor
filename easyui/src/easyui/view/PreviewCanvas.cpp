#include "PreviewCanvas.h"

#include "frame/SettingCfg.h"

namespace eui
{

BEGIN_EVENT_TABLE(PreviewCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, PreviewCanvas::onTimer)
END_EVENT_TABLE()

const float PreviewCanvas::VIEW_WIDTH = 1024;
const float PreviewCanvas::VIEW_HEIGHT = 768;

PreviewCanvas::PreviewCanvas(d2d::EditPanel* stage, d2d::PlayControl& control,
							 const std::vector<const d2d::ISprite*>& sprites)
	: d2d::OrthoCanvas(stage)
	, m_timer(this, TIMER_ID)
	, m_control(control)
	, m_sprites(sprites)
{
	m_timer.Start(100);

	SettingCfg* cfg = SettingCfg::Instance();
	float scale = std::min(cfg->m_view_width / VIEW_WIDTH, cfg->m_view_height / VIEW_HEIGHT);
	m_scale_mt.scale(scale, scale);
}

void PreviewCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
	for (int i = 0, n = m_sprites.size(); i < n; ++i) {
		m_sprites[i]->getSymbol().reloadTexture();
	}
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->ReloadTexture();
	}
}

void PreviewCanvas::onDraw()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		const d2d::ISprite* sprite = m_sprites[i];
		// 		if (!sprite->visiable)
		// 			continue;

		const d2d::Vector& pos = sprite->getPosition();

		d2d::Matrix inv_mt = sprite->GetTransInvMatrix();
		d2d::Matrix translate_mt;
		translate_mt.translate(pos.x, pos.y);
		d2d::Matrix mt = translate_mt * (m_scale_mt * inv_mt);

		d2d::SpriteDraw::drawSprite(sprite, mt, sprite->multiCol, sprite->addCol);
	}
}

void PreviewCanvas::onTimer(wxTimerEvent& event)
{
	bool refresh = m_control.update();
	if (refresh) {
		Refresh();
	}
}

}