#pragma once

#include <wx/wx.h>

namespace eanim
{
	class TimeLinePanel : public wxPanel
	{
	public:
		TimeLinePanel(wxWindow* parent);

		void clear();

	private:
		void initLayout();

		void initTitle(wxSizer* sizer);
		void initSplitterWindow(wxSizer* sizer);

	private:
		static const float SASH_GRAVITY_VERT;

	}; // TimeLinePanel
}

