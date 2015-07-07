#ifndef _DRAG2D_PERSPECT_CANVAS_H_
#define _DRAG2D_PERSPECT_CANVAS_H_

#include "OnePassCanvas.h"

namespace d2d
{

class EditPanel;

class PerspectCanvas : public OnePassCanvas
{
public:
	PerspectCanvas(EditPanel* editPanel);

private:
	virtual void OnSize(int w, int h);

}; // PerspectCanvas

}

#endif // _DRAG2D_PERSPECT_CANVAS_H_