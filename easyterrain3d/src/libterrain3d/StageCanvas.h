#ifndef _EASYTERRAIN3D_STAGE_CANVAS_H_
#define _EASYTERRAIN3D_STAGE_CANVAS_H_

#include <easy3d.h>

namespace eterrain3d
{

class IDemo;

class StageCanvas : public e3d::StageCanvas
{
public:
	StageCanvas(wxWindow* stage_wnd, ee::EditPanelImpl* stage);

protected:
	virtual void OnDrawSprites() const;

private:
	void OnTimer(wxTimerEvent& event);

	enum
	{
		TIMER_ID = 1000
	};

private:
	wxTimer m_timer;

	IDemo* m_demo;

	DECLARE_EVENT_TABLE()

}; // StageCanvas

}

#endif // _EASYTERRAIN3D_STAGE_CANVAS_H_