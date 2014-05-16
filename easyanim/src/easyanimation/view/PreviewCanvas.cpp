#include "PreviewCanvas.h"
#include "PreviewSettings.h"

#include "frame/Context.h"
#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"

#include "../libanim/Tools.h"
#include "../libanim/PreviewControl.h"

namespace eanim
{

BEGIN_EVENT_TABLE(PreviewCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, PreviewCanvas::onTimer)
END_EVENT_TABLE()

PreviewCanvas::PreviewCanvas(d2d::EditPanel* stage, const PlaySettings& settings,
							 anim::PreviewControl& control)
	: d2d::OrthoCanvas(stage)
	, m_timer(this, TIMER_ID)
	, m_control(control)
	, m_settings(settings)
{
	m_timer.Start(10);
}

void PreviewCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
	static_cast<d2d::LibraryPanel*>(Context::Instance()->library)->reloadTexture();
	d2d::Shader::Instance()->release();
}

void PreviewCanvas::onDraw()
{
	drawStageData();
}

void PreviewCanvas::onTimer(wxTimerEvent& event)
{
	bool refresh = false;
	if (!m_settings.isStop) {
		refresh = m_control.update();
	}

	if (m_control.frame() >= Context::Instance()->layers.getFrameCount())
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
		sprites[i]->release();
}

void PreviewCanvas::getCurrSprites(std::vector<d2d::ISprite*>& sprites) const
{
	for (size_t i = 0, n = Context::Instance()->layers.size(); i < n; ++i)
	{
		Layer* layer = Context::Instance()->layers.getLayer(i);

		KeyFrame *currFrame = layer->getCurrKeyFrame(m_control.frame()),
			*nextFrame = layer->getNextKeyFrame(m_control.frame());
		if (!currFrame)
			continue;

		if (!currFrame->hasClassicTween() || !nextFrame)
		{
			for (int i = 0, n = currFrame->size(); i < n; ++i) {
				sprites.push_back(currFrame->getSprite(i)->clone());
			}
		}
		else
		{
			assert(m_control.frame() >= currFrame->getTime() && m_control.frame() < nextFrame->getTime());
			float process = (float) (m_control.frame() - currFrame->getTime()) / (nextFrame->getTime() - currFrame->getTime());
//			anim::Tools::getTweenSprites(currFrame->getAllSprites(), nextFrame->getAllSprites(), sprites, process);

			currFrame->getTweenSprites(currFrame, nextFrame, sprites, process);
		}
	}
}

} // eanim