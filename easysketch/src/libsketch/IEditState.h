#ifndef _EASYSKETCH_INTERFACE_EDIT_STATE_H_
#define _EASYSKETCH_INTERFACE_EDIT_STATE_H_

namespace esketch
{

class IEditState
{
public:
	virtual ~IEditState() {}

	virtual void OnMousePress(const sm::ivec2& pos) {}
	virtual void OnMouseRelease(const sm::ivec2& pos) {}
	virtual void OnMouseMove(const sm::ivec2& pos) {}

	virtual void OnMouseWheelRotation(const sm::ivec2& pos, int direction) {}

}; // IEditState

}

#endif // _EASYSKETCH_INTERFACE_EDIT_STATE_H_