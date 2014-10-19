#ifndef _LIBSKETCH_EDIT_CAMERA_OP_H_
#define _LIBSKETCH_EDIT_CAMERA_OP_H_

#include <drag2d.h>
#include <easy3d.h>

namespace libsketch
{

class IEditState;

class EditCameraOP : public d2d::AbstractEditOP
{
public:
	EditCameraOP(d2d::EditPanel* stage);
	virtual ~EditCameraOP();

	virtual bool onKeyDown(int keyCode);
	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseRightUp(int x, int y);
	virtual bool onMouseMove(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onMouseWheelRotation(int x, int y, int direction);

private:
	e3d::StageCanvas* m_canvas;

	IEditState* m_curr;
	IEditState *m_translate, *m_rotate, *m_zoom;

}; // EditCameraOP

}

#endif // _LIBSKETCH_EDIT_CAMERA_OP_H_