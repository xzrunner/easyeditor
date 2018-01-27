#ifndef _EASYTERRAIN3D_STAGE_CANVAS_H_
#define _EASYTERRAIN3D_STAGE_CANVAS_H_

#include <ee3/StageCanvas.h>

namespace ee3 { class StagePanel; }

namespace eterrain3d
{

class IDemo;

class StageCanvas : public ee3::StageCanvas
{
public:
	StageCanvas(ee3::StagePanel* stage);

protected:
	virtual void OnDrawSprites() const override;

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