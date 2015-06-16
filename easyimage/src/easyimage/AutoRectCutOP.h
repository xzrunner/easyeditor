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

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseDrag(int x, int y);
	virtual bool OnMouseLeftDClick(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

	const RectMgr& getRectMgr() const { return m_rects; }
	RectMgr& getRectMgr() { return m_rects; }

private:
	RectMgr m_rects;

	d2d::Rect* m_selected;

	d2d::Vector m_last_pos;

}; // AutoRectCutOP

}

#endif // _EASYIMAGE_AUTO_RECT_CUT_OP_H_