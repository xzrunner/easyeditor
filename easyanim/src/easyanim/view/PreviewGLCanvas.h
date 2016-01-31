#pragma once


#include <wx/wx.h>

namespace eanim
{
	class Sprite;
	class LayersMgr;

	class PreviewGLCanvas : public ee::OrthoCanvas
	{
	public:
		struct PlaySetting
		{
			bool isCirculate;
			bool isStop;

			PlaySetting()
			{
				isCirculate = true;
				isStop = false;
			}
		};

	public:
		PreviewGLCanvas(ee::EditPanel* editPanel, LayersMgr* layers, int fps = 24);

		PlaySetting& getPlaySetting();

	protected:
		virtual void initGL();
		virtual void onDraw();

		void onTimer(wxTimerEvent& event);

	private:
		void drawStageData();

		void getCurrSprites(std::vector<Sprite*>& sprites) const;
		void getTweenSprites(const std::vector<Sprite*>& start, const std::vector<Sprite*>& end, 
			std::vector<Sprite*>& tween, float process) const;
		bool isTweenMatched(Sprite& s0, Sprite& s1) const;
		void getTweenSprite(Sprite* start, Sprite* end, Sprite* tween, float process) const;

	private:
		enum
		{
			TIMER_ID = 1000
		};

	private:
		LayersMgr* m_layersMgr;

		wxTimer m_timer;
		int m_currFrame;

		PlaySetting m_setting;

		DECLARE_EVENT_TABLE()

	}; // PreviewGLCanvas
}

