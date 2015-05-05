#ifndef _LIBSHAPE_DRAW_PENCIL_POLYGON_OP_H_
#define _LIBSHAPE_DRAW_PENCIL_POLYGON_OP_H_

#include "DrawCurveOP.h"

namespace libshape
{

class DrawPencilPolygonCMPT;

class DrawPencilPolygonOP : public DrawCurveOP
{
public:
	DrawPencilPolygonOP(d2d::EditPanel* editPanel, d2d::MultiShapesImpl* shapesImpl,
		d2d::OneFloatValue* simplify, DrawPencilPolygonCMPT* cmpt);

	virtual bool onMouseLeftUp(int x, int y);

private:
	void NewPolygon(const std::vector<d2d::Vector>& poly);

	void UnionPolygon(const std::vector<d2d::Vector>& poly);
	void DifferencePolygon(const std::vector<d2d::Vector>& poly);

private:
	enum Type
	{
		e_normal = 0,
		e_add,
		e_del
	};

private:
	d2d::MultiShapesImpl* m_shapesImpl;

	d2d::OneFloatValue* m_simplify;

	DrawPencilPolygonCMPT* m_cmpt;

}; // DrawPencilPolygonOP

}

#endif // _LIBSHAPE_DRAW_PENCIL_POLYGON_OP_H_