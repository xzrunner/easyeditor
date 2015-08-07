#ifndef _DRAG2D_ONE_PASS_CANVAS_H_
#define _DRAG2D_ONE_PASS_CANVAS_H_

#include "IStageCanvas.h"

namespace d2d
{

class OnePassCanvas : public IStageCanvas
{
public:
	OnePassCanvas(wxWindow* stage_wnd, EditPanelImpl* stage);

protected:
	virtual void OnDrawWhole() const;

}; // OnePassCanvas

}

#endif // _DRAG2D_ONE_PASS_CANVAS_H_