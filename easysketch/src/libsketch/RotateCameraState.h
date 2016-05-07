#ifndef _EASYSKETCH_ROTATE_CAMERA_STATE_H_
#define _EASYSKETCH_ROTATE_CAMERA_STATE_H_

#include "IEditState.h"

#include <easy3d.h>

namespace esketch
{

class RotateCameraState : public IEditState
{
public:
	RotateCameraState(e3d::StageCanvas* canvas);

	virtual void OnMousePress(const sm::ivec2& pos);
	virtual void OnMouseRelease(const sm::ivec2& pos);
	virtual void OnMouseMove(const sm::ivec2& pos);

private:
	e3d::StageCanvas* m_canvas;

	sm::ivec2 m_last_pos;

}; // RotateCameraState

}

#endif // _EASYSKETCH_ROTATE_CAMERA_STATE_H_