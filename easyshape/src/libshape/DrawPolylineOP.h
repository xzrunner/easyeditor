#ifndef _EASYSHAPE_DRAW_POLYLINE_OP_H_
#define _EASYSHAPE_DRAW_POLYLINE_OP_H_

#include <ee/ZoomViewOP.h>

#include <SM_Vector.h>
#include <cu/cu_stl.h>

#include <vector>

namespace eshape
{

class DrawPolylineOP : public ee::ZoomViewOP
{
public:
	DrawPolylineOP(wxWindow* wnd, ee::EditPanelImpl* stage, bool isClosed);

	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseLeftDClick(int x, int y) override;

	virtual bool OnDraw() const override;
	virtual bool Clear() override;

	void SetLoop(bool is_loop) {
		m_is_closed = is_loop;
	}

private:
	bool ShouldFixPos() const;

protected:
	mutable CU_VEC<sm::vec2> m_polyline;
	sm::vec2 m_curr_pos;

private:
	bool m_is_closed;

	// todo
	friend class EditPolylineImpl;

}; // DrawPolylineOP

}

#endif // _EASYSHAPE_DRAW_POLYLINE_OP_H_