#ifndef _LIBSHAPE_DRAW_CURVE_OP_H_
#define _LIBSHAPE_DRAW_CURVE_OP_H_

#include <drag2d.h>

namespace libshape
{

class DrawCurveOP : public d2d::ZoomViewOP
{
public:
	DrawCurveOP(wxWindow* wnd, d2d::EditPanelImpl* stage);

	virtual bool OnMouseLeftDown(int x, int y);
	virtual bool OnMouseLeftUp(int x, int y);
	virtual bool OnMouseDrag(int x, int y);

	virtual bool OnDraw() const;
	virtual bool Clear();

protected:
	std::vector<d2d::Vector> m_curve;

private:
	d2d::Vector m_firstPos;
	bool m_startDraw;

	bool m_straight_mode;

}; // DrawCurveOP

}

#endif // _LIBSHAPE_DRAW_CURVE_OP_H_