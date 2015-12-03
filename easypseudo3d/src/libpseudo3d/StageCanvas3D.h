#ifndef _EASYPSEUDO3D_STAGE_CANVAS_3D_H_
#define _EASYPSEUDO3D_STAGE_CANVAS_3D_H_

#include <drag2d.h>
#include <easy3d.h>

namespace epseudo3d
{

class IScene;

class StageCanvas3D : public e3d::StageCanvas
{
public:
	StageCanvas3D(wxWindow* stage_wnd, d2d::EditPanelImpl* stage);

	void StoreScene(const char* filename) const;
	void LoadScene(const char* filename);

	void GetScreenSize(int* width, int* height) const;

protected:
	virtual void OnDrawSprites() const;
	//virtual void OnDrawDC() const;

	void OnTimer(wxTimerEvent& event);

private:
	enum
	{
		TIMER_ID = 1000
	};

private:
	wxTimer m_timer;

	IScene* m_scene;

	DECLARE_EVENT_TABLE()

}; // StageCanvas3D

}

#endif // _EASYPSEUDO3D_STAGE_CANVAS_3D_H_
