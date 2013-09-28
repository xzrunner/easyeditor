#pragma once

#include <drag2d.h>
#include <wx/wx.h>

namespace libanim
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
		PreviewCanvas(d2d::EditPanel* stage, d2d::LibraryPanel* library,
			const d2d::AnimSymbol* symbol);

		PlaySetting& getPlaySetting();

	protected:
		virtual void initGL();
		virtual void onDraw();

		void onTimer(wxTimerEvent& event);

	private:
		enum
		{
			TIMER_ID = 1000
		};

	private:
		wxTimer m_timer;
		int m_currFrame;

		PlaySetting m_setting;

		d2d::LibraryPanel* m_library;

		const d2d::AnimSymbol* m_symbol;

		DECLARE_EVENT_TABLE()

	}; // PreviewCanvas
}

