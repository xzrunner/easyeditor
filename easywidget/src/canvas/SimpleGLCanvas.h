#ifndef _EASYWIDGET_SIMPLE_GL_CANVAS_H_
#define _EASYWIDGET_SIMPLE_GL_CANVAS_H_

#include <easygui.h>

namespace ewidget
{

class SimpleGLCanvas : public egui::GLCanvas
{
public:
	SimpleGLCanvas(egui::Window* parent);

protected:
	virtual void OnSize(int w, int h);

}; // SimpleGLCanvas

}

#endif // _EASYWIDGET_SIMPLE_GL_CANVAS_H_