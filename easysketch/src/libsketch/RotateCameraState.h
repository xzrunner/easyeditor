#ifndef _LIBSKETCH_ROTATE_CAMERA_STATE_H_
#define _LIBSKETCH_ROTATE_CAMERA_STATE_H_

#include "IArrangeState.h"

#include <easy3d.h>

namespace libsketch
{

class RotateCameraState : public IArrangeState
{
public:
	RotateCameraState(e3d::StageCanvas* canvas);

	virtual void OnMousePress(const ivec2& pos);
	virtual void OnMouseRelease(const ivec2& pos);
	virtual void OnMouseMove(const ivec2& pos);

private:
	e3d::StageCanvas* m_canvas;

	bool m_is_open;

	Quaternion m_ori, m_pre_ori;
	ivec2 m_first_pos;

}; // RotateCameraState

}

#endif // _LIBSKETCH_ROTATE_CAMERA_STATE_H_