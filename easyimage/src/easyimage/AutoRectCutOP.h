#ifndef _EASYIMAGE_AUTO_RECT_CUT_OP_H_
#define _EASYIMAGE_AUTO_RECT_CUT_OP_H_

#include "interfaces.h"
#include "RectMgr.h"

#include <drag2d.h>

namespace eimage
{

class AutoRectCutOP : public d2d::ZoomViewOP
{
public:
	AutoRectCutOP(d2d::EditPanel* editpanel);

	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseDrag(int x, int y);
	virtual bool onMouseLeftDClick(int x, int y);

	virtual bool onDraw() const;

	const RectMgr& getRectMgr() const { return m_rects; }
	RectMgr& getRectMgr() { return m_rects; }

private:
	RectMgr m_rects;

	d2d::Rect* m_selected;

	d2d::Vector m_last_pos;

}; // AutoRectCutOP

}

#endif // _EASYIMAGE_AUTO_RECT_CUT_OP_H_