#ifndef _LIBSHAPE_DRAW_POLYLINE_OP_H_
#define _LIBSHAPE_DRAW_POLYLINE_OP_H_

#include <drag2d.h>

namespace libshape
{

class DrawPolylineOP : public d2d::ZoomViewOP
{
public:
	DrawPolylineOP(d2d::EditPanel* editPanel, bool isClosed);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseRightDown(int x, int y);
	virtual bool OnMouseMove(int x, int y);
	virtual bool OnMouseLeftDClick(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

	void SetLoop(bool is_loop) {
		m_isClosed = is_loop;
	}

private:
	bool ShouldFixPos() const;

protected:
	mutable std::vector<d2d::Vector> m_polyline;
	d2d::Vector m_currPos;

private:
	bool m_isClosed;

	// todo
	friend class EditPolylineImpl;

}; // DrawPolylineOP

}

#endif // _LIBSHAPE_DRAW_POLYLINE_OP_H_