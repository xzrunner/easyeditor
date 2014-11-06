#ifndef _E3D_CONTROL_CAMERA_OP_H_
#define _E3D_CONTROL_CAMERA_OP_H_

#include <drag2d.h>

namespace e3d
{

class StageCanvas;

class ControlCameraOP : public d2d::AbstractEditOP
{
public:
	ControlCameraOP(d2d::EditPanel* stage);

	virtual bool onKeyDown(int keyCode);
	virtual bool onMouseMove(int x, int y);

private:
	StageCanvas* m_canvas;

}; // ControlCameraOP

}

#endif // _E3D_CONTROL_CAMERA_OP_H_