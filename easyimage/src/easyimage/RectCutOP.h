#ifndef _EASYIMAGE_RECT_CUT_OP_H_
#define _EASYIMAGE_RECT_CUT_OP_H_

#include "interfaces.h"
#include "RectMgr.h"



namespace eimage
{

class RectCutCMPT;
class StagePanel;

class RectCutOP : public ee::ZoomViewOP
{
public:
	RectCutOP(RectCutCMPT* cmpt, StagePanel* stage);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseRightUp(int x, int y);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

	const RectMgr& getRectMgr() const { return m_rects; }
	RectMgr& getRectMgr() { return m_rects; }

	std::string getImageFilepath() const;
	void loadImageFromFile(const std::string& filepath);

	RectCutCMPT* getEditCMPT() const { return m_cmpt; }

private:
	void drawCaptureLine() const;

	void fixedPos(ee::Vector& pos) const;

private:
	RectCutCMPT* m_cmpt;

	StagePanel* m_stage;

	ee::Vector m_firstPos, m_currPos;

	ee::Vector m_captured;

	RectMgr::Node m_nodeSelected;
	ee::Rect* m_rectSelected;

	RectMgr m_rects;

}; // RectCutOP 

}

#endif // _EASYIMAGE_RECT_CUT_OP_H_