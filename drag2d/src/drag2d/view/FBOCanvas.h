#ifndef _DRAG2D_FBO_CANVAS_H_
#define _DRAG2D_FBO_CANVAS_H_

#include <wx/glcanvas.h>

#include "Screen.h"

namespace d2d
{

class EditPanel;
class Camera;

class FBOCanvas : public wxGLCanvas
{
public:
	FBOCanvas(EditPanel* stage);
	virtual ~FBOCanvas();

	void SetCurrentCanvas();

protected:
	virtual void InitGL();
	virtual void OnSize(int w, int h) = 0;
	virtual void OnDraw() const = 0;

private:
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);

	void CreateFBO(int w, int h);
	void ReleaseFBO();

	void onEraseBackground(wxEraseEvent& event);
protected:
	Camera* m_camera;

	Screen m_screen;

private:
	bool m_inited;

	wxGLContext* m_context;

	typedef unsigned int GLuint;
	GLuint m_tex;
	GLuint m_fbo;

	DECLARE_EVENT_TABLE()

}; // FBOCanvas

}

#endif // _DRAG2D_FBO_CANVAS_H_