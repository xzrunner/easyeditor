#ifndef _DRAG2D_FBO_CANVAS_H_
#define _DRAG2D_FBO_CANVAS_H_

#include "IStageCanvas.h"

namespace d2d
{

class FBOCanvas : public IStageCanvas
{
public:
	FBOCanvas(EditPanel* stage);
	virtual ~FBOCanvas();

protected:
	virtual void OnSize(int w, int h);
	virtual void OnDrawWhole() const;

private:
	void CreateFBO(int w, int h);
	void ReleaseFBO();

private:
	typedef unsigned int GLuint;
	GLuint m_tex;
	GLuint m_fbo;

}; // FBOCanvas

}

#endif // _DRAG2D_FBO_CANVAS_H_