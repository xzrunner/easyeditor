#ifndef _DRAG2D_FBO_CANVAS_H_
#define _DRAG2D_FBO_CANVAS_H_

#include "IStageCanvas.h"

#include "render/FBO.h"

namespace d2d
{

class FBOCanvas : public IStageCanvas
{
public:
	FBOCanvas(EditPanel* stage);

protected:
	virtual void OnSize(int w, int h);
	virtual void OnDrawWhole() const;

private:
	FBO m_fbo;

}; // FBOCanvas

}

#endif // _DRAG2D_FBO_CANVAS_H_