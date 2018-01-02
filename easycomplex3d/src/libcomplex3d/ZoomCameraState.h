#ifndef _EASYCOMPLEX3D_ZOOM_CAMERA_STATE_H_
#define _EASYCOMPLEX3D_ZOOM_CAMERA_STATE_H_

#include "IEditState.h"

namespace e3d { class StageCanvas; }

namespace ecomplex3d
{

class ZoomCameraState : public IEditState
{
public:
	ZoomCameraState(e3d::StageCanvas* canvas);

	virtual void OnMouseWheelRotation(const sm::ivec2& pos, int direction) override;

private:
	e3d::StageCanvas* m_canvas;

}; // ZoomCameraState

}

#endif // _EASYCOMPLEX3D_ZOOM_CAMERA_STATE_H_
