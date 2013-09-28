#pragma once

#include <drag2d.h>
#include <wx/wx.h>

namespace eanim
{
	class PreviewCanvas : public d2d::OrthoCanvas
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
		PreviewCanvas(d2d::EditPanel* stage);

		PlaySetting& getPlaySetting();

	protected:
		virtual void initGL();
		virtual void onDraw();

		void onTimer(wxTimerEvent& event);

	private:
		void drawStageData();

		void getCurrSprites(std::vector<d2d::ISprite*>& sprites) const;

	private:
		enum
		{
			TIMER_ID = 1000
		};

	private:
		wxTimer m_timer;
		int m_currFrame;

		PlaySetting m_setting;

		DECLARE_EVENT_TABLE()

	}; // PreviewCanvas
}

