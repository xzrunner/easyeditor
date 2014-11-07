#ifndef _LIBSKETCH_ROTATE_CAMERA_STATE_H_
#define _LIBSKETCH_ROTATE_CAMERA_STATE_H_

#include "IEditState.h"

#include <easy3d.h>

namespace libsketch
{

class RotateCameraState : public IEditState
{
public:
	RotateCameraState(e3d::StageCanvas* canvas);

	virtual void OnMousePress(const ivec2& pos);
	virtual void OnMouseRelease(const ivec2& pos);
	virtual void OnMouseMove(const ivec2& pos);

private:
	e3d::StageCanvas* m_canvas;

	ivec2 m_last_pos;

}; // RotateCameraState

}

#endif // _LIBSKETCH_ROTATE_CAMERA_STATE_H_