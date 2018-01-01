#ifndef _EASY3D_CONTROL_CAMERA_OP_H_
#define _EASY3D_CONTROL_CAMERA_OP_H_

#include <ee/EditOP.h>

#include <SM_Vector.h>

namespace e3d
{

class StageCanvas;

class ControlCameraOP : public ee::EditOP
{
public:
	ControlCameraOP(wxWindow* wnd, ee::EditPanelImpl* stage);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;

private:
	StageCanvas* m_canvas;

	sm::ivec2 m_last_pos;

}; // ControlCameraOP

}

#endif // _EASY3D_CONTROL_CAMERA_OP_H_