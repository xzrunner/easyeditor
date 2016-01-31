#ifndef _EASYEDITOR_ZOOM_VIEW_STATE_H_
#define _EASYEDITOR_ZOOM_VIEW_STATE_H_

#include "EditOPState.h"

namespace ee
{

class EditPanelImpl;

class ZoomViewState : public EditOPState
{
public:
	ZoomViewState(EditPanelImpl* stage);
	virtual ~ZoomViewState();
	
	virtual bool OnMouseWheelRotation(int x, int y, int direction);

private:
	EditPanelImpl* m_stage;

}; // ZoomViewState

}

#endif // _EASYEDITOR_ZOOM_VIEW_STATE_H_