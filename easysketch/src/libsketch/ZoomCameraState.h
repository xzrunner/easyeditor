#ifndef _LIBSKETCH_ZOOM_CAMERA_STATE_H_
#define _LIBSKETCH_ZOOM_CAMERA_STATE_H_

#include "IEditState.h"

#include <easy3d.h>

namespace libsketch
{

class ZoomCameraState : public IEditState
{
public:
	ZoomCameraState(e3d::StageCanvas* canvas);

	virtual void OnMouseWheelRotation(const ivec2& pos, int direction);

private:
	e3d::StageCanvas* m_canvas;

}; // ZoomCameraState

}

#endif // _LIBSKETCH_ZOOM_CAMERA_STATE_H_