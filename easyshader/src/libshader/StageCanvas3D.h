#ifndef _ESHADER_STAGE_CANVAS3D_H_
#define _ESHADER_STAGE_CANVAS3D_H_

#include <easysketch.h>

namespace eshader
{

class StageCanvas3D : public libsketch::StageCanvas
{
public:
	StageCanvas3D(wxWindow* stage_wnd, d2d::EditPanelImpl* stage, 
		d2d::MultiSpritesImpl* stage, d2d::LibraryPanel* library);

private:
	void OnTimer(wxTimerEvent& event);

private:
	enum
	{
		TIMER_ID = 1000
	};

private:
	wxTimer m_timer;

	DECLARE_EVENT_TABLE()

}; // StageCanvas3D

}

#endif // _ESHADER_STAGE_CANVAS3D_H_