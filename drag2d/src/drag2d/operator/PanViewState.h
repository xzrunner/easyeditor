#ifndef _DRAG2D_PAN_VIEW_STATE_H_
#define _DRAG2D_PAN_VIEW_STATE_H_

#include "IEditOPState.h"

#include "common/Vector.h"

namespace d2d
{

class EditPanel;

class PanViewState : public IEditOPState
{
public:
	PanViewState(EditPanel* stage);

	virtual void Bind();
	virtual void UnBind();

	virtual bool OnMousePress(int x, int y);
	virtual bool OnMouseRelease(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

private:
	EditPanel* m_stage;

	Vector m_last_pos;

}; // PanViewState

}

#endif // _DRAG2D_PAN_VIEW_STATE_H_