#ifndef _DRAG2D_ORTHO_CANVAS_H_
#define _DRAG2D_ORTHO_CANVAS_H_

#include "TwoPassCanvas.h"

namespace d2d
{

class EditPanel;

class OrthoCanvas : public TwoPassCanvas
{
public:
	OrthoCanvas(EditPanel* editPanel);

protected:
	virtual void OnSize(int w, int h);

}; // OrthoCanvas

}

#endif // _DRAG2D_ORTHO_CANVAS_H_