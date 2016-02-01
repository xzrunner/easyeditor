#ifndef _EASYIMAGE_RECT_CUT_OP_H_
#define _EASYIMAGE_RECT_CUT_OP_H_

#include "RectMgr.h"

#include <ee/ZoomViewOP.h>

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

	const RectMgr& GetRectMgr() const { return m_rects; }
	RectMgr& GetRectMgr() { return m_rects; }

	std::string GetImageFilepath() const;
	void LoadImageFromFile(const std::string& filepath);

	RectCutCMPT* GetEditCMPT() const { return m_cmpt; }

private:
	void DrawCaptureLine() const;

	void FixedPos(ee::Vector& pos) const;

private:
	RectCutCMPT* m_cmpt;

	StagePanel* m_stage;

	ee::Vector m_first_pos, m_curr_pos;

	ee::Vector m_captured;

	RectMgr::Node m_node_selected;
	ee::Rect* m_rect_selected;

	RectMgr m_rects;

}; // RectCutOP 

}

#endif // _EASYIMAGE_RECT_CUT_OP_H_