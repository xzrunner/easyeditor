#ifndef _EASYPSEUDO3D_STAGE_CANVAS_H_
#define _EASYPSEUDO3D_STAGE_CANVAS_H_

#include <drag2d.h>
#include <easy3d.h>

namespace epseudo3d
{

class IScene;

class StageCanvas : public e3d::StageCanvas
{
public:
	StageCanvas(d2d::EditPanel* stage);

	void StoreScene() const;
	void LoadScene();

protected:
	virtual void initGL();
	virtual void onDraw();
	virtual void OnDrawDC() const;

	void onTimer(wxTimerEvent& event);

private:
	void InitCamera();

private:
	enum
	{
		TIMER_ID = 1000
	};

private:
	wxTimer m_timer;

	IScene* m_scene;

	DECLARE_EVENT_TABLE()

}; // StageCanvas

}

#endif // _EASYPSEUDO3D_STAGE_CANVAS_H_
