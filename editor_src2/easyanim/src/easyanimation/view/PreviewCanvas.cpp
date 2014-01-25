#include "PreviewCanvas.h"

#include "frame/Context.h"
#include "dataset/KeyFrame.h"
#include "dataset/Layer.h"
#include "dataset/LayersMgr.h"

#include "../libanim/Tools.h"

namespace eanim
{

BEGIN_EVENT_TABLE(PreviewCanvas, d2d::OrthoCanvas)
	EVT_TIMER(TIMER_ID, PreviewCanvas::onTimer)
END_EVENT_TABLE()

PreviewCanvas::PreviewCanvas(d2d::EditPanel* stage)
	: d2d::OrthoCanvas(stage)
	, m_timer(this, TIMER_ID)
{
	m_timer.Start(1000 / Context::Instance()->fps);
	m_currFrame = 1;
}

PreviewCanvas::PlaySetting& PreviewCanvas::getPlaySetting()
{
	return m_setting;
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
	if (!m_setting.isStop)
		++m_currFrame;

	if (m_currFrame >= Context::Instance()->layers.getFrameCount())
	{
		if (m_setting.isCirculate) 
			m_currFrame = 1;
		else 
			--m_currFrame;
	}

	Refresh();
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

		KeyFrame *currFrame = layer->getCurrKeyFrame(m_currFrame),
			*nextFrame = layer->getNextKeyFrame(m_currFrame);
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
			assert(m_currFrame >= currFrame->getTime() && m_currFrame < nextFrame->getTime());
			float process = (float) (m_currFrame - currFrame->getTime()) / (nextFrame->getTime() - currFrame->getTime());
//			libanim::Tools::getTweenSprites(currFrame->getAllSprites(), nextFrame->getAllSprites(), sprites, process);

			KeyFrame::getTweenSprites(currFrame, nextFrame, sprites, process);
		}
	}
}
} // eanim