#ifndef _E3D_CONTROL_CAMERA_OP_H_
#define _E3D_CONTROL_CAMERA_OP_H_



#include "Vector.h"

namespace e3d
{

class StageCanvas;

class ControlCameraOP : public ee::EditOP
{
public:
	ControlCameraOP(wxWindow* wnd, ee::EditPanelImpl* stage);

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