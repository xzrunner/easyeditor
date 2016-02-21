#ifndef _EASYSKETCH_INTERFACE_EDIT_STATE_H_
#define _EASYSKETCH_INTERFACE_EDIT_STATE_H_

#include <ee/Vector3D.h>

namespace esketch
{

class IEditState
{
public:
	virtual ~IEditState() {}

	virtual void OnMousePress(const ee::ivec2& pos) {}
	virtual void OnMouseRelease(const ee::ivec2& pos) {}
	virtual void OnMouseMove(const ee::ivec2& pos) {}

	virtual void OnMouseWheelRotation(const ee::ivec2& pos, int direction) {}

}; // IEditState

}

#endif // _EASYSKETCH_INTERFACE_EDIT_STATE_H_