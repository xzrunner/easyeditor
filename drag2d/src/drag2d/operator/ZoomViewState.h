#ifndef _DRAG2D_ZOOM_VIEW_STATE_H_
#define _DRAG2D_ZOOM_VIEW_STATE_H_

#include "IEditOPState.h"

namespace d2d
{

class EditPanel;

class ZoomViewState : public IEditOPState
{
public:
	ZoomViewState(EditPanel* stage) : m_stage(stage) {}
	
	virtual bool OnMouseWheelRotation(int x, int y, int direction);

private:
	EditPanel* m_stage;

}; // ZoomViewState

}

#endif // _DRAG2D_ZOOM_VIEW_STATE_H_