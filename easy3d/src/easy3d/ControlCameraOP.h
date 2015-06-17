#ifndef _E3D_CONTROL_CAMERA_OP_H_
#define _E3D_CONTROL_CAMERA_OP_H_

#include <drag2d.h>

#include "Vector.h"

namespace e3d
{

class StageCanvas;

class ControlCameraOP : public d2d::AbstractEditOP
{
public:
	ControlCameraOP(d2d::EditPanel* stage);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseDrag(int x, int y);
	virtual bool OnMouseMove(int x, int y);

private:
	StageCanvas* m_canvas;

	ivec2 m_last_pos;

}; // ControlCameraOP

}

#endif // _E3D_CONTROL_CAMERA_OP_H_