#ifndef _EASYEDITOR_EDIT_OP_STATE_H_
#define _EASYEDITOR_EDIT_OP_STATE_H_

namespace ee
{

class EditOPState
{
public:
	virtual ~EditOPState() {}

	virtual void Bind() {}
	virtual void UnBind() {}

	virtual bool OnMousePress(int x, int y) { return false; }
	virtual bool OnMouseRelease(int x, int y) { return false; }
	virtual bool OnMouseMove(int x, int y) { return false; }
	virtual bool OnMouseDrag(int x, int y) { return false; }

	virtual bool OnMouseWheelRotation(int x, int y, int direction) { return false; }

}; // EditOPState

}

#endif // _EASYEDITOR_EDIT_OP_STATE_H_