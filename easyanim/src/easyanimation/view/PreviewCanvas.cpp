#include "PreviewCanvas.h"
#include "PreviewSettings.h"

#include "frame/Controller.h"
#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"

#include "../libanim/Tools.h"

namespace eanim
{

BEGIN_EVENT_TABLE(PreviewCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, PreviewCanvas::onTimer)
END_EVENT_TABLE()

PreviewCanvas::PreviewCanvas(d2d::EditPanel* stage, const PlaySettings& settings,
							 d2d::PlayControl& control, Controller* ctrl)
	: d2d::OrthoCanvas(stage)
	, m_timer(this, TIMER_ID)
	, m_control(control)
	, m_settings(settings)
	, m_ctrl(ctrl)
{
	m_timer.Start(30);
}

void PreviewCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
	m_ctrl->GetLibraryPanel()->reloadTexture();
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->ReloadTexture();
	}

	resetViewport();
}

void PreviewCanvas::onDraw()
{
	float xedge = GetSize().GetWidth() * 0.5f;
	float yedge = GetSize().GetHeight() * 0.5f;
	d2d::PrimitiveDraw::cross(d2d::Vector(0,0), xedge, yedge, d2d::LIGHT_GREY);

	drawStageData();

#ifdef _DEBUG 
	if (d2d::Config::Instance()->IsUseDTex()) {
		d2d::DynamicTexAndFont::Instance()->DebugDraw();
	}	
#endif
}

void PreviewCanvas::onTimer(wxTimerEvent& event)
{
	bool refresh = false;
	if (!m_settings.isStop) {
		refresh = m_control.update();
	}

	if (m_control.frame() >= m_ctrl->GetLayers().getFrameCount())
	{
		if (m_settings.isCirculate) {
			m_control.reset();
		}
		else {
			m_control.decrease();
		}
	}

	if (refresh) {
		Refresh();
	}
}

// todo: waste time!
void PreviewCanvas::drawStageData()
{
	std::vector<d2d::ISprite*> sprites;
	getCurrSprites(sprites);
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		d2d::SpriteDraw::drawSprite(sprites[i]);

	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->Release();
}

void PreviewCanvas::getCurrSprites(std::vector<d2d::ISprite*>& sprites) const
{
	LayersMgr& layers = m_ctrl->GetLayers();
	for (size_t i = 0, n = layers.size(); i < n; ++i)
	{
		Layer* layer = layers.getLayer(i);

		KeyFrame *currFrame = layer->getCurrKeyFrame(m_control.frame()),
			*nextFrame = layer->getNextKeyFrame(m_control.frame());
		if (!currFrame)
			continue;

		if (!currFrame) 
		{
			for (int i = 0, n = currFrame->size(); i < n; ++i) {
				sprites.push_back(currFrame->getSprite(i)->clone());
			}
		}
		else if (!currFrame->hasClassicTween())
		{
			for (int i = 0, n = currFrame->size(); i < n; ++i) {
				sprites.push_back(currFrame->getSprite(i)->clone());
			}
		}
		else
		{
			assert(m_control.frame() >= currFrame->getTime() && m_control.frame() < nextFrame->getTime());
			float process = (float) (m_control.frame() - currFrame->getTime()) / (nextFrame->getTime() - currFrame->getTime());
//			libanim::Tools::getTweenSprites(currFrame->getAllSprites(), nextFrame->getAllSprites(), sprites, process);

			currFrame->getTweenSprites(currFrame, nextFrame, sprites, process);
		}
	}
}

} // eanim