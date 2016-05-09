#ifndef _EASYEDITOR_PAN_VIEW_STATE_H_
#define _EASYEDITOR_PAN_VIEW_STATE_H_

#include "EditOPState.h"

#include <SM_Vector.h>

namespace ee
{

class EditPanelImpl;

class PanViewState : public EditOPState
{
public:
	PanViewState(EditPanelImpl* stage);
	virtual ~PanViewState();

	virtual void Bind();
	virtual void UnBind();

	virtual bool OnMousePress(int x, int y);
	virtual bool OnMouseRelease(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

private:
	EditPanelImpl* m_stage;

	sm::vec2 m_last_pos;
	bool m_last_pos_valid;

}; // PanViewState

}

#endif // _EASYEDITOR_PAN_VIEW_STATE_H_