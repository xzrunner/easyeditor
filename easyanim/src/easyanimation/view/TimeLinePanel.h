#ifndef _EASYANIM_TIMELINE_PANEL_H_
#define _EASYANIM_TIMELINE_PANEL_H_

#include <wx/wx.h>

namespace eanim
{

class TimeLinePanel : public wxPanel
{
public:
	TimeLinePanel(wxWindow* parent);

private:
	void InitLayout();

	void InitTitle(wxSizer* sizer);
	void InitSplitterWindow(wxSizer* sizer);

}; // TimeLinePanel

}

#endif // _EASYANIM_TIMELINE_PANEL_H_