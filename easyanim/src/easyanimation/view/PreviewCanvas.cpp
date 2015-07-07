#include "PreviewCanvas.h"
#include "PreviewSettings.h"

#include "frame/Controller.h"
#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"

#include <easyanim.h>

namespace eanim
{

PreviewCanvas::PreviewCanvas(d2d::EditPanel* stage, const PlaySettings& settings,
							 d2d::PlayControl& control, Controller* ctrl)
	: d2d::DynamicStageCanvas(stage)
	, m_control(control)
	, m_settings(settings)
	, m_ctrl(ctrl)
{
}

void PreviewCanvas::InitGL()
{
	d2d::DynamicStageCanvas::InitGL();
	m_ctrl->GetLibraryPanel()->ReloadTexture();
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->ReloadTexture();
	}

	ResetViewport();
}

void PreviewCanvas::OnDrawSprites() const
{
	float xedge = GetSize().GetWidth() * 0.5f;
	float yedge = GetSize().GetHeight() * 0.5f;
	d2d::PrimitiveDraw::cross(d2d::Vector(0,0), xedge, yedge, d2d::LIGHT_GREY);

	DrawStageData();

#ifdef _DEBUG 
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->DebugDraw();
	}	
#endif
}

void PreviewCanvas::OnTimer()
{
	m_control.update();
	if (m_control.frame() <= m_ctrl->GetLayers().getFrameCount()) {
		return;
	}

	if (m_settings.isCirculate) {
		m_control.reset();
	} else {
		m_control.decrease();
	}
}

// todo: waste time!
void PreviewCanvas::DrawStageData() const
{
	std::vector<d2d::ISprite*> sprites;
	GetCurrSprites(sprites);

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		d2d::SpriteRenderer::Instance()->Draw(sprites[i]);

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->Release();
}

void PreviewCanvas::GetCurrSprites(std::vector<d2d::ISprite*>& sprites) const
{
	LayersMgr& layers = m_ctrl->GetLayers();
	for (size_t i = 0, n = layers.size(); i < n; ++i)
	{
		Layer* layer = layers.getLayer(i);

		KeyFrame *currFrame = layer->GetCurrKeyFrame(m_control.frame()),
			*nextFrame = layer->GetNextKeyFrame(m_control.frame());
		if (!currFrame)
			continue;

		if (!currFrame) 
		{
			for (int i = 0, n = currFrame->Size(); i < n; ++i) {
				sprites.push_back(currFrame->GetSprite(i)->Clone());
			}
		}
		else if (!currFrame->HasClassicTween())
		{
			for (int i = 0, n = currFrame->Size(); i < n; ++i) {
				sprites.push_back(currFrame->GetSprite(i)->Clone());
			}
		}
		else
		{
			assert(m_control.frame() >= currFrame->GetTime() && m_control.frame() < nextFrame->GetTime());
			float process = (float) (m_control.frame() - currFrame->GetTime()) / (nextFrame->GetTime() - currFrame->GetTime());
//			libanim::Tools::getTweenSprites(currFrame->getAllSprites(), nextFrame->getAllSprites(), sprites, process);

			currFrame->GetTweenSprite(currFrame, nextFrame, sprites, process);
		}
	}
}

} // eanim