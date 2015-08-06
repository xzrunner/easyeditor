#ifndef _LIBSHAPE_DRAW_PENCIL_POLYGON_OP_H_
#define _LIBSHAPE_DRAW_PENCIL_POLYGON_OP_H_

#include "DrawCurveOP.h"

namespace libshape
{

class DrawPencilPolygonCMPT;

class DrawPencilPolygonOP : public DrawCurveOP
{
public:
	DrawPencilPolygonOP(wxWindow* wnd, d2d::EditPanelImpl* stage, d2d::MultiShapesImpl* shapesImpl,
		d2d::OneFloatValue* simplify, DrawPencilPolygonCMPT* cmpt);

	virtual bool OnKeyDown(int keyCode);
	virtual bool OnMouseLeftUp(int x, int y);

private:
	void NewPolygon(const std::vector<d2d::Vector>& poly);

	void UnionPolygon(const std::vector<d2d::Vector>& poly);
	void DifferencePolygon(const std::vector<d2d::Vector>& poly);
	void IntersectionPolygon(const std::vector<d2d::Vector>& poly);
	void XorPolygon(const std::vector<d2d::Vector>& poly);

	void PrepareSubjectPaths(std::vector<std::vector<d2d::Vector> >& paths) const;
	void ReplacePolygons(const std::vector<std::vector<d2d::Vector> >& paths);

private:
	enum Type
	{
		e_normal = 0,
		e_union,
		e_difference,
		e_intersection,
		e_xor
	};

private:
	d2d::MultiShapesImpl* m_shapesImpl;

	d2d::OneFloatValue* m_simplify;

	DrawPencilPolygonCMPT* m_cmpt;

}; // DrawPencilPolygonOP

}

#endif // _LIBSHAPE_DRAW_PENCIL_POLYGON_OP_H_