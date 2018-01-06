#pragma once

#include <SM_Vector.h>

namespace e3d
{

class EditOpState
{
public:
	virtual ~EditOpState() {}
	
	virtual void OnMousePress(const sm::ivec2& pos) {}
	virtual void OnMouseRelease(const sm::ivec2& pos) {}
	virtual void OnMouseMove(const sm::ivec2& pos) {}
	virtual void OnMouseDrag(const sm::ivec2& pos) {}

	virtual void OnDirectionKeyDown(int type) {}

}; // EditOpState

}