#pragma once

namespace ee0
{

class EditOpState
{
public:
	virtual ~EditOpState() {}
	
	virtual void Bind() {}
	virtual void UnBind() {}

	virtual bool OnMousePress(int x, int y) { return false; }
	virtual bool OnMouseRelease(int x, int y) { return false; }
	virtual bool OnMouseMove(int x, int y) { return false; }
	virtual bool OnMouseDrag(int x, int y) { return false; }

	virtual bool OnDirectionKeyDown(int type) { return false; }

	virtual bool OnMouseWheelRotation(int x, int y, int direction) { return false; }

	virtual bool OnDraw() const { return false; };
	virtual bool Clear() { return false; }

}; // EditOpState

}