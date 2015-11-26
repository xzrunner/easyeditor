#ifndef _DRAG2D_TWO_PASS_CANVAS_H_
#define _DRAG2D_TWO_PASS_CANVAS_H_

#include "IStageCanvas.h"

namespace d2d
{

class TwoPassCanvas : public IStageCanvas
{
public:
	TwoPassCanvas(wxWindow* stage_wnd, EditPanelImpl* stage);

protected:
	virtual void InitGL();
	virtual void OnSize(int w, int h);
	virtual void OnDrawWhole() const;

}; // TwoPassCanvas

}

#endif // _DRAG2D_TWO_PASS_CANVAS_H_