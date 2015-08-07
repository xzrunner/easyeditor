#ifndef _DRAG2D_ORTHO_CANVAS_H_
#define _DRAG2D_ORTHO_CANVAS_H_

#include "TwoPassCanvas.h"

namespace d2d
{

class OrthoCanvas : public TwoPassCanvas
{
public:
	OrthoCanvas(wxWindow* stage_wnd, EditPanelImpl* stage);

protected:
	virtual void OnSize(int w, int h);

}; // OrthoCanvas

}

#endif // _DRAG2D_ORTHO_CANVAS_H_