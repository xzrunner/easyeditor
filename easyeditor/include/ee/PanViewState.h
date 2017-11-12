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

	virtual void Bind() override;
	virtual void UnBind() override;

	virtual bool OnMousePress(int x, int y) override;
	virtual bool OnMouseRelease(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

private:
	EditPanelImpl* m_stage;

	sm::vec2 m_last_pos;

}; // PanViewState

}

#endif // _EASYEDITOR_PAN_VIEW_STATE_H_