#ifndef _EASYSHAPE_DRAW_POLYLINE_OP_H_
#define _EASYSHAPE_DRAW_POLYLINE_OP_H_

#include <ee/ZoomViewOP.h>

#include <SM_Vector.h>

#include <vector>

namespace eshape
{

class DrawPolylineOP : public ee::ZoomViewOP
{
public:
	DrawPolylineOP(wxWindow* wnd, ee::EditPanelImpl* stage, bool isClosed);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseLeftDClick(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

	void SetLoop(bool is_loop) {
		m_is_closed = is_loop;
	}

private:
	bool ShouldFixPos() const;

protected:
	mutable std::vector<sm::vec2> m_polyline;
	sm::vec2 m_curr_pos;
	bool m_curr_pos_valid;

private:
	bool m_is_closed;

	// todo
	friend class EditPolylineImpl;

}; // DrawPolylineOP

}

#endif // _EASYSHAPE_DRAW_POLYLINE_OP_H_