#include "StageCanvas.h"
#include "StagePanel.h"
#include "Symbol.h"
#include "Settings.h"

#include <easyanim.h>

namespace ecomplex
{
	BEGIN_EVENT_TABLE(StageCanvas, d2d::OrthoCanvas)
		EVT_TIMER(TIMER_ID, StageCanvas::onTimer)
	END_EVENT_TABLE()

	StageCanvas::StageCanvas(StagePanel* editPanel)
		: d2d::OrthoCanvas(editPanel)
		, m_timer(this, TIMER_ID)
		, m_stage(editPanel)
		, m_background(NULL)
		, m_stat(1)
	{
		m_timer.Start(1000 / 30);

		m_bgStyle.color.set(0.8f, 0.8f, 0.8f);
		m_clipboxStyle.color.set(0, 0.8f, 0);
	}

	StageCanvas::~StageCanvas()
	{
		if (m_background)
		{
			m_background->Release();
		}
	}

	void StageCanvas::initGL()
	{
		d2d::OrthoCanvas::initGL();
		m_stage->getSymbol()->reloadTexture();
// 		d2d::DynamicTexture::Instance()->ReloadTexture();
// 		d2d::DynamicFont::Instance()->ReloadTexture();

		if (d2d::Config::Instance()->IsUseDTex()) {
			d2d::DynamicTexAndFont::Instance()->ReloadTexture();
		}

		resetViewport();
	}

	void StageCanvas::onDraw()
	{
		m_stat.Begin();

		drawBackground();

  		std::vector<d2d::ISprite*> sprites;
  		m_stage->traverseSprites(d2d::FetchAllVisitor<d2d::ISprite>(sprites));
  
  		for (size_t i = 0, n = sprites.size(); i < n; ++i)
  		{
  			d2d::ISprite* sprite = sprites[i];
  			if (!sprite->visiable)
  				continue;
  			d2d::SpriteDraw::drawSprite(sprite);
  		}

  		d2d::PrimitiveDraw::rect(m_stage->getSymbol()->m_clipbox, m_clipboxStyle);
  
   		if (Settings::bVisibleBGCross)
   		{
   			const float EDGE = 100;
   			d2d::PrimitiveDraw::cross(d2d::Vector(0,0), EDGE, EDGE, d2d::LIGHT_GREY);
   		}
 
     	m_stage->drawEditTemp();

		m_stat.End();

#ifdef _DEBUG 
//		d2d::DynamicTexture::Instance()->DebugDraw();
//		d2d::DynamicFont::Instance()->DebugDraw();
		if (d2d::Config::Instance()->IsUseDTex()) {
			d2d::DynamicTexAndFont::Instance()->DebugDraw();
		}
#endif

		m_stat.DrawTime(m_screen);
	}

	void StageCanvas::onTimer(wxTimerEvent& event)
	{
		Refresh();
	}

	void StageCanvas::drawBackground() const
	{
		if (m_background)
		{
			d2d::Matrix mt;
			m_background->draw(mt, m_background->getRegion());
		}

		if (Settings::bVisibleBGRect)
		{
			d2d::PrimitiveDraw::rect(d2d::Vector(0, 0), 1024 * 0.5f, 768 * 0.5f, m_bgStyle);
		}
	}
}