#ifndef _LIBSHAPE_DRAW_CURVE_OP_H_
#define _LIBSHAPE_DRAW_CURVE_OP_H_

#include <drag2d.h>

namespace libshape
{

class DrawCurveOP : public d2d::ZoomViewOP
{
public:
	DrawCurveOP(d2d::EditPanel* editPanel);

	virtual bool onMouseLeftDown(int x, int y);
	virtual bool onMouseLeftUp(int x, int y);
	virtual bool onMouseDrag(int x, int y);

	virtual bool onDraw() const;
	virtual bool clear();

protected:
	std::vector<d2d::Vector> m_curve;

private:
	d2d::Vector m_firstPos;
	bool m_startDraw;

}; // DrawCurveOP

}

#endif // _LIBSHAPE_DRAW_CURVE_OP_H_