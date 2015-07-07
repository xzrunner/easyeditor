#ifndef _DRAG2D_ORTHO_CANVAS_H_
#define _DRAG2D_ORTHO_CANVAS_H_

#include "OnePassCanvas.h"

namespace d2d
{

class EditPanel;

class OrthoCanvas : public OnePassCanvas
{
public:
	OrthoCanvas(EditPanel* editPanel);

protected:
	virtual void OnSize(int w, int h);

}; // OrthoCanvas

}

#endif // _DRAG2D_ORTHO_CANVAS_H_