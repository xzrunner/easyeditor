#ifndef _DRAG2D_I_EDIT_OP_STATE_H_
#define _DRAG2D_I_EDIT_OP_STATE_H_

namespace d2d
{

class IEditOPState
{
public:
	virtual ~IEditOPState() {}

	virtual void Bind() {}
	virtual void UnBind() {}

	virtual bool OnMousePress(int x, int y) { return false; }
	virtual bool OnMouseRelease(int x, int y) { return false; }
	virtual bool OnMouseMove(int x, int y) { return false; }
	virtual bool OnMouseDrag(int x, int y) { return false; }

	virtual bool OnMouseWheelRotation(int x, int y, int direction) { return false; }

}; // IEditOPState

}

#endif // _DRAG2D_I_EDIT_OP_STATE_H_