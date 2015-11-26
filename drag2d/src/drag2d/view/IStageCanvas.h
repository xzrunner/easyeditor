#ifndef _DRAG2D_INTERFACE_STAGE_CANVAS_H_
#define _DRAG2D_INTERFACE_STAGE_CANVAS_H_

#include <wx/glcanvas.h>
#include <wx/timer.h>

#include "Screen.h"
#include "common/Color.h"
#include "common/Object.h"
#include "message/Observer.h"

#include <vector>

namespace d2d
{

class EditPanelImpl;

class IStageCanvas : public wxGLCanvas, public Object, public Observer
{
public:
	IStageCanvas(wxWindow* stage_wnd, EditPanelImpl* stage);
	virtual ~IStageCanvas();

	//
	//	interface Observer
	//
	virtual void Notify(int sj_id, void* ud);

	void EnableObserve(bool enable) { m_observe_enable = enable; }

	void ResetInitState();		// Another IStageCanvas closed, refresh the under one

	void SetBgColor(const Colorf& color);

	void SetCurrentCanvas();

	Camera* GetCamera() { return m_camera; }
	const Camera* GetCamera() const { return m_camera; }

	bool IsDirty() const { return m_dirty; }

	void SetCamDirty() { m_cam_dirty = true; }

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

protected:
	virtual void InitGL();
	virtual void OnSize(int w, int h) = 0;
	virtual void OnDrawWhole() const = 0;
	virtual void OnDrawSprites() const = 0;
	//virtual void OnDrawDC() const {}

	virtual void OnTimer() {}

	bool IsInited() const { return m_inited; }

private:
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);
	//void OnEraseBackground(wxEraseEvent& event);
	void OnMouse(wxMouseEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnKeyUp(wxKeyEvent& event);

	void OnTimer(wxTimerEvent& event);

	void OnKillFocus(wxFocusEvent& event);

protected:
	EditPanelImpl* m_stage;

	Colorf m_bg_color;

	int m_width, m_height;

	Screen m_screen;
	Camera* m_camera;

private:
	enum
	{
		TIMER_ID = 9999
	};

private:
	wxGLContext* m_context;

	std::vector<Subject*> m_subjects;
	bool m_observe_enable;

	bool m_inited;

	bool m_dirty;
	bool m_cam_dirty;

	wxTimer m_timer;

	int m_version;

	DECLARE_EVENT_TABLE()

}; // IStageCanvas

}

#endif // _DRAG2D_INTERFACE_STAGE_CANVAS_H_