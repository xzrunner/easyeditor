#ifndef _EASYIMAGE_AUTO_RECT_CUT_OP_H_
#define _EASYIMAGE_AUTO_RECT_CUT_OP_H_

#include "RectMgr.h"

#include <ee/ZoomViewOP.h>

namespace eimage
{

class AutoRectCutOP : public ee::ZoomViewOP
{
public:
	AutoRectCutOP(wxWindow* wnd, ee::EditPanelImpl* stage);

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;
	virtual bool OnMouseLeftDClick(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

	const RectMgr& getRectMgr() const { return m_rects; }
	RectMgr& getRectMgr() { return m_rects; }

private:
	RectMgr m_rects;

	sm::rect* m_selected;

	sm::vec2 m_last_pos;

}; // AutoRectCutOP

}

#endif // _EASYIMAGE_AUTO_RECT_CUT_OP_H_