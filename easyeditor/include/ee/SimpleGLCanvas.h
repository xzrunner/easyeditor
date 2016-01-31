#ifndef _EASYEDITOR_SIMPLE_GL_CANVAS_H_
#define _EASYEDITOR_SIMPLE_GL_CANVAS_H_

#include <wx/glcanvas.h>

namespace ee
{

class SimpleGLCanvas : public wxGLCanvas
{
public:
	SimpleGLCanvas(wxWindow* parent);
	virtual ~SimpleGLCanvas();

protected:
	virtual void OnDraw() const = 0;

private:
	void OnSize(wxSizeEvent& event);
	void OnPaint(wxPaintEvent& event);

private:
	wxGLContext* m_context;

	DECLARE_EVENT_TABLE()

}; // SimpleGLCanvas

}

#endif // _EASYEDITOR_SIMPLE_GL_CANVAS_H_