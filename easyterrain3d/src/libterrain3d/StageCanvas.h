#ifndef _EASYTERRAIN3D_STAGE_CANVAS_H_
#define _EASYTERRAIN3D_STAGE_CANVAS_H_

#include <drag2d.h>
#include <easy3d.h>

namespace libterrain3d
{

class IDemo;

class StageCanvas : public e3d::StageCanvas
{
public:
	StageCanvas(d2d::EditPanel* stage);

protected:
	virtual void InitGL();
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