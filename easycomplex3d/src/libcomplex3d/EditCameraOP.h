#ifndef _EASYCOMPLEX3D_EDIT_CAMERA_OP_H_
#define _EASYCOMPLEX3D_EDIT_CAMERA_OP_H_

#include <ee/EditOP.h>

namespace e3d { class StageCanvas; }

namespace ecomplex3d
{

class IEditState;

class EditCameraOP : public ee::EditOP
{
public:
	EditCameraOP(wxWindow* wnd, ee::EditPanelImpl* stage);
	virtual ~EditCameraOP();

	virtual bool OnKeyDown(int keyCode) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnMouseWheelRotation(int x, int y, int direction) override;

private:
	e3d::StageCanvas* m_canvas;

	IEditState* m_curr;
	IEditState *m_translate, *m_rotate, *m_zoom;

}; // EditCameraOP

}

#endif // _EASYCOMPLEX3D_EDIT_CAMERA_OP_H_
