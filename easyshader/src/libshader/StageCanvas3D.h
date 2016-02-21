#ifndef _EASYSHADER_STAGE_CANVAS3D_H_
#define _EASYSHADER_STAGE_CANVAS3D_H_

#include <easysketch.h>

namespace eshader
{

class StageCanvas3D : public esketch::StageCanvas
{
public:
	StageCanvas3D(wxWindow* stage_wnd, ee::EditPanelImpl* stage, 
		ee::MultiSpritesImpl* sprite_impl, ee::LibraryPanel* library);

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

#endif // _EASYSHADER_STAGE_CANVAS3D_H_