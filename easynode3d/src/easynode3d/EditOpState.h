#pragma once

#include <SM_Vector.h>

namespace enode3d
{

class EditOpState
{
public:
	virtual ~EditOpState() {}
	
	virtual void OnMousePress(const sm::vec2& pos) {}
	virtual void OnMouseRelease(const sm::vec2& pos) {}
	virtual void OnMouseMove(const sm::vec2& pos) {}
	virtual void OnMouseDrag(const sm::vec2& pos) {}

	virtual void OnDirectionKeyDown(int type) {}

	virtual void OnMouseWheelRotation(int x, int y, int direction) {}

}; // EditOpState

}