#ifndef _LIBSKETCH_EDIT_CAMERA_OP_H_
#define _LIBSKETCH_EDIT_CAMERA_OP_H_

#include <drag2d.h>

namespace libsketch
{

class IEditState;

class EditCameraOP : public d2d::AbstractEditOP
{
public:
	EditCameraOP(d2d::EditPanel* stage);

	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseRightUp(int x, int y);
	virtual bool onMouseDrag(int x, int y);

private:
	IEditState* m_curr;
	IEditState *m_translate, *m_rotate, *m_zoom;

}; // EditCameraOP

}

#endif // _LIBSKETCH_EDIT_CAMERA_OP_H_