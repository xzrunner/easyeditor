#ifndef _EASYIMAGE_RECT_CUT_OP_H_
#define _EASYIMAGE_RECT_CUT_OP_H_

#include <drag2d.h>

#include "RectMgr.h"

namespace eimage
{

class StagePanel;

class RectCutOP : public d2d::ZoomViewOP
{
public:
	RectCutOP(StagePanel* stage);

	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseRightUp(int x, int y);
	virtual bool onMouseMove(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onDraw() const;
	virtual bool clear();

	const std::vector<d2d::Rect*>& getAllRect() const {
		return m_rects.getAllRect();
	}

private:
	void drawCaptureLine() const;

	void fixedPos(d2d::Vector& pos) const;

private:
	StagePanel* m_stage;

	d2d::Vector m_firstPos, m_currPos;

	d2d::Vector m_captured;

	RectMgr::Node m_nodeSelected;
	const d2d::Rect* m_rectSelected;

	RectMgr m_rects;

}; // RectCutOP 

}

#endif // _EASYIMAGE_RECT_CUT_OP_H_