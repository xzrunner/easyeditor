#ifndef _EASYGUI_GL_CANVAS_H_
#define _EASYGUI_GL_CANVAS_H_

namespace egui
{

class Window;
class GLCanvasImpl;

class GLCanvas
{
public:
	GLCanvas(Window* parent);
	virtual ~GLCanvas();

	virtual void SetSize(int w, int h);

protected:
	virtual void OnDraw() const {}
	virtual void OnSize(int w, int h) {}

private:
	GLCanvasImpl* m_impl;
	
	friend class GLCanvasImpl;

}; // GLCanvas

}

#endif // _EASYGUI_GL_CANVAS_H_
