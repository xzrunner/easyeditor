#ifndef _EASYIMAGE_RECT_CUT_OP_H_
#define _EASYIMAGE_RECT_CUT_OP_H_

#include "interfaces.h"
#include "RectMgr.h"

#include <drag2d.h>

namespace eimage
{

class RectCutCMPT;
class StagePanel;

class RectCutOP : public d2d::ZoomViewOP
{
public:
	RectCutOP(RectCutCMPT* cmpt, StagePanel* stage);

	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseRightDown(int x, int y);
	virtual bool onMouseRightUp(int x, int y);
	virtual bool onMouseMove(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onDraw() const;
	virtual bool clear();

	const RectMgr& getRectMgr() const { return m_rects; }
	RectMgr& getRectMgr() { return m_rects; }

	std::string getImageFilepath() const;
	void loadImageFromFile(const std::string& filepath);

	RectCutCMPT* getEditCMPT() const { return m_cmpt; }

private:
	void drawCaptureLine() const;

	void fixedPos(d2d::Vector& pos) const;

private:
	RectCutCMPT* m_cmpt;

	StagePanel* m_stage;

	d2d::Vector m_firstPos, m_currPos;

	d2d::Vector m_captured;

	RectMgr::Node m_nodeSelected;
	d2d::Rect* m_rectSelected;

	RectMgr m_rects;

}; // RectCutOP 

}

#endif // _EASYIMAGE_RECT_CUT_OP_H_