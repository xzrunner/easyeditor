#ifndef _EASY3D_CONTROL_CAMERA_OP_H_
#define _EASY3D_CONTROL_CAMERA_OP_H_

#include <ee/EditOP.h>

#include <memory>

namespace e3d
{

class StageCanvas;
class EditOpState;

class ControlCameraOP : public ee::EditOP
{
public:
	ControlCameraOP(wxWindow* wnd, ee::EditPanelImpl* stage);

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseWheelRotation(int x, int y, int direction) override;

private:
	StageCanvas* m_canvas;

	std::unique_ptr<EditOpState> m_op_state = nullptr;

}; // ControlCameraOP

}

#endif // _EASY3D_CONTROL_CAMERA_OP_H_