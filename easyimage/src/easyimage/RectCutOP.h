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
	virtual bool OnMouseLeftDClick(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

	const RectMgr& GetRectMgr() const { return m_rects; }
	RectMgr& GetRectMgr() { return m_rects; }

	std::string GetImageFilepath() const;
	void LoadImageFromFile(const std::string& filepath);

	RectCutCMPT* GetEditCMPT() const { return m_cmpt; }

	const sm::vec2& GetCenter() const { return m_center.pos; }
	void SetCenter(const sm::vec2& center) { m_center.pos = center; }

private:
	void DrawCaptureLine() const;

	void FixedPos(sm::vec2& pos) const;

private:
	struct Cross
	{
		static const int RADIUS = 10;
		static const int LENGTH = 100;

		Cross();
		void Draw() const;
		bool Contain(const sm::vec2& p) const;

		sm::vec2 pos;

		bool selected;

	}; // Cross

private:
	RectCutCMPT* m_cmpt;

	StagePanel* m_stage;

	sm::vec2 m_first_pos, m_curr_pos;

	sm::vec2 m_captured;

	RectMgr::Node m_node_selected;
	sm::rect* m_rect_selected;

	RectMgr m_rects;

	Cross m_center;

}; // RectCutOP 

}

#endif // _EASYIMAGE_RECT_CUT_OP_H_