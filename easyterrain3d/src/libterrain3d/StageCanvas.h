#ifndef _EASYTERRAIN3D_STAGE_CANVAS_H_
#define _EASYTERRAIN3D_STAGE_CANVAS_H_

#include <easynode3d/StageCanvas.h>

namespace enode3d { class StagePanel; }

namespace eterrain3d
{

class IDemo;

class StageCanvas : public enode3d::StageCanvas
{
public:
	StageCanvas(enode3d::StagePanel* stage);

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