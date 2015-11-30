#include "PreviewCanvas.h"
#include "PreviewSettings.h"

#include "frame/Controller.h"
#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"

#include <easyanim.h>

namespace eanim
{

PreviewCanvas::PreviewCanvas(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, const PlaySettings& settings,
							 d2d::PlayControl& control, Controller* ctrl)
	: d2d::OrthoCanvas(stage_wnd, stage)
	, m_control(control)
	, m_settings(settings)
	, m_ctrl(ctrl)
{
}

void PreviewCanvas::InitGL()
{
	d2d::OrthoCanvas::InitGL();

	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DrawCallBatching::Instance()->ReloadBegin();
	}
	d2d::ImageMgr::Instance()->Traverse(d2d::ReloadTextureVisitor<d2d::Image>());
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DrawCallBatching::Instance()->ReloadEnd();
	}

	d2d::ResetViewportSJ::Instance()->Reset();
}

void PreviewCanvas::OnDrawSprites() const
{
	float xedge = GetSize().GetWidth() * 0.5f;
	float yedge = GetSize().GetHeight() * 0.5f;
	d2d::PrimitiveDraw::cross(d2d::Vector(0,0), xedge, yedge, d2d::LIGHT_GREY);

	DrawStageData();
}

void PreviewCanvas::OnTimer()
{
	m_control.update();
	d2d::SetCanvasDirtySJ::Instance()->SetDirty();

	if (m_control.frame() <= m_ctrl->GetLayers().GetFrameCount()) {
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
	for (size_t i = 0, n = layers.Size(); i < n; ++i)
	{
		Layer* layer = layers.GetLayer(i);

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
		else if (!currFrame->HasClassicTween() || !nextFrame)
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